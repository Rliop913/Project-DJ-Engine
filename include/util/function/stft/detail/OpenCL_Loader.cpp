#define CL_TARGET_OPENCL_VERSION 300

#include "OpenCL_Loader.hpp"

#include <CL/opencl.h>

#include <atomic>
#include <mutex>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <dlfcn.h>
#endif

#ifndef CL_PLATFORM_NOT_FOUND_KHR
#define CL_PLATFORM_NOT_FOUND_KHR -1001
#endif

namespace {

#define PDJE_OPENCL_RUNTIME_SYMBOLS(X)                                      \
    X(clGetPlatformIDs)                                                     \
    X(clGetPlatformInfo)                                                    \
    X(clGetDeviceIDs)                                                       \
    X(clGetDeviceInfo)                                                      \
    X(clRetainDevice)                                                       \
    X(clReleaseDevice)                                                      \
    X(clCreateContext)                                                      \
    X(clCreateContextFromType)                                              \
    X(clGetContextInfo)                                                     \
    X(clRetainContext)                                                      \
    X(clReleaseContext)                                                     \
    X(clCreateCommandQueue)                                                 \
    X(clCreateCommandQueueWithProperties)                                   \
    X(clGetCommandQueueInfo)                                                \
    X(clRetainCommandQueue)                                                 \
    X(clReleaseCommandQueue)                                                \
    X(clFlush)                                                              \
    X(clFinish)                                                             \
    X(clCreateProgramWithSource)                                            \
    X(clBuildProgram)                                                       \
    X(clGetProgramInfo)                                                     \
    X(clGetProgramBuildInfo)                                                \
    X(clRetainProgram)                                                      \
    X(clReleaseProgram)                                                     \
    X(clCreateKernel)                                                       \
    X(clSetKernelArg)                                                       \
    X(clGetKernelInfo)                                                      \
    X(clGetKernelWorkGroupInfo)                                             \
    X(clRetainKernel)                                                       \
    X(clReleaseKernel)                                                      \
    X(clCreateBuffer)                                                       \
    X(clCreateBufferWithProperties)                                         \
    X(clGetMemObjectInfo)                                                   \
    X(clRetainMemObject)                                                    \
    X(clReleaseMemObject)                                                   \
    X(clEnqueueReadBuffer)                                                  \
    X(clEnqueueWriteBuffer)                                                 \
    X(clEnqueueNDRangeKernel)                                               \
    X(clWaitForEvents)                                                      \
    X(clGetEventInfo)                                                       \
    X(clRetainEvent)                                                        \
    X(clReleaseEvent)                                                       \
    X(clGetExtensionFunctionAddress)                                        \
    X(clGetExtensionFunctionAddressForPlatform)

#if defined(_WIN32)
using OpenCLLibraryHandle = HMODULE;
#else
using OpenCLLibraryHandle = void *;
#endif

struct OpenCLRuntimeDispatch {
#define PDJE_DECLARE_DISPATCH_MEMBER(name) decltype(&::name) name = nullptr;
    PDJE_OPENCL_RUNTIME_SYMBOLS(PDJE_DECLARE_DISPATCH_MEMBER)
#undef PDJE_DECLARE_DISPATCH_MEMBER
};

OpenCLRuntimeDispatch gOpenCLDispatch{};
OpenCLLibraryHandle   gOpenCLLibraryHandle = nullptr;
std::once_flag        gOpenCLRuntimeInitOnce;
std::atomic<bool>     gOpenCLRuntimeReady{ false };

#if defined(_WIN32)
OpenCLLibraryHandle
OpenCLibraryOpen() noexcept
{
    return ::LoadLibraryA("OpenCL.dll");
}

void *
OpenCLibrarySymbol(OpenCLLibraryHandle handle,
                   const char         *symbolName) noexcept
{
    if (handle == nullptr || symbolName == nullptr) {
        return nullptr;
    }

    return reinterpret_cast<void *>(::GetProcAddress(handle, symbolName));
}

void
OpenCLibraryClose(OpenCLLibraryHandle handle) noexcept
{
    if (handle != nullptr) {
        ::FreeLibrary(handle);
    }
}
#elif defined(__linux__)
OpenCLLibraryHandle
OpenCLibraryOpen() noexcept
{
    OpenCLLibraryHandle handle = ::dlopen("libOpenCL.so.1", RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
        handle = ::dlopen("libOpenCL.so", RTLD_NOW | RTLD_LOCAL);
    }
    return handle;
}

void *
OpenCLibrarySymbol(OpenCLLibraryHandle handle,
                   const char         *symbolName) noexcept
{
    if (handle == nullptr || symbolName == nullptr) {
        return nullptr;
    }

    return ::dlsym(handle, symbolName);
}

void
OpenCLibraryClose(OpenCLLibraryHandle handle) noexcept
{
    if (handle != nullptr) {
        ::dlclose(handle);
    }
}
#else
OpenCLLibraryHandle
OpenCLibraryOpen() noexcept
{
    return nullptr;
}

void *
OpenCLibrarySymbol(OpenCLLibraryHandle,
                   const char *) noexcept
{
    return nullptr;
}

void
OpenCLibraryClose(OpenCLLibraryHandle) noexcept
{}
#endif

bool
ResolveOpenCLRuntimeSymbols(OpenCLLibraryHandle  handle,
                            OpenCLRuntimeDispatch &dispatch) noexcept
{
#define PDJE_RESOLVE_DISPATCH_SYMBOL(name)                                  \
    dispatch.name = reinterpret_cast<decltype(dispatch.name)>(              \
        OpenCLibrarySymbol(handle, #name));                                 \
    if (dispatch.name == nullptr) {                                         \
        return false;                                                       \
    }

    PDJE_OPENCL_RUNTIME_SYMBOLS(PDJE_RESOLVE_DISPATCH_SYMBOL)

#undef PDJE_RESOLVE_DISPATCH_SYMBOL
    return true;
}

bool
SmokeProbeOpenCLPlatforms(const OpenCLRuntimeDispatch &dispatch) noexcept
{
    cl_uint platformCount = 0;
    const cl_int err = dispatch.clGetPlatformIDs(0, nullptr, &platformCount);

    return err == CL_SUCCESS && platformCount > 0;
}

void
InitializeOpenCLRuntime() noexcept
{
#if !defined(_WIN32) && !defined(__linux__)
    return;
#else
    OpenCLRuntimeDispatch dispatch;
    OpenCLLibraryHandle   handle = OpenCLibraryOpen();

    if (handle == nullptr) {
        return;
    }

    if (!ResolveOpenCLRuntimeSymbols(handle, dispatch) ||
        !SmokeProbeOpenCLPlatforms(dispatch)) {
        OpenCLibraryClose(handle);
        return;
    }

    gOpenCLDispatch      = dispatch;
    gOpenCLLibraryHandle = handle;
    gOpenCLRuntimeReady.store(true, std::memory_order_release);
#endif
}

const OpenCLRuntimeDispatch *
GetOpenCLRuntimeDispatch() noexcept
{
    if (!gOpenCLRuntimeReady.load(std::memory_order_acquire)) {
        return nullptr;
    }

    return &gOpenCLDispatch;
}

cl_int
GetUnavailableOpenCLError() noexcept
{
    return CL_INVALID_OPERATION;
}

cl_int
GetUnavailablePlatformError() noexcept
{
    return CL_PLATFORM_NOT_FOUND_KHR;
}

#define PDJE_OPENCL_INT_WRAPPER(name, signature, arguments, failureCode)    \
    extern "C" CL_API_ENTRY cl_int CL_API_CALL                               \
    name signature                                                           \
    {                                                                        \
        const auto *dispatch = GetOpenCLRuntimeDispatch();                   \
        if (dispatch == nullptr || dispatch->name == nullptr) {              \
            return failureCode;                                              \
        }                                                                    \
        return dispatch->name arguments;                                     \
    }

#define PDJE_OPENCL_HANDLE_WRAPPER(returnType, name, signature, arguments,   \
                                   errcode_ret_name)                         \
    extern "C" CL_API_ENTRY returnType CL_API_CALL                           \
    name signature                                                           \
    {                                                                        \
        const auto *dispatch = GetOpenCLRuntimeDispatch();                   \
        if (dispatch == nullptr || dispatch->name == nullptr) {              \
            if (errcode_ret_name != nullptr) {                               \
                *errcode_ret_name = GetUnavailableOpenCLError();             \
            }                                                                \
            return nullptr;                                                  \
        }                                                                    \
        return dispatch->name arguments;                                     \
    }

#define PDJE_OPENCL_VOIDPTR_WRAPPER(name, signature, arguments)              \
    extern "C" CL_API_ENTRY void * CL_API_CALL                               \
    name signature                                                           \
    {                                                                        \
        const auto *dispatch = GetOpenCLRuntimeDispatch();                   \
        if (dispatch == nullptr || dispatch->name == nullptr) {              \
            return nullptr;                                                  \
        }                                                                    \
        return dispatch->name arguments;                                     \
    }

PDJE_OPENCL_INT_WRAPPER(clGetPlatformIDs,
                        (cl_uint          num_entries,
                         cl_platform_id * platforms,
                         cl_uint *        num_platforms),
                        (num_entries, platforms, num_platforms),
                        GetUnavailablePlatformError())

PDJE_OPENCL_INT_WRAPPER(clGetPlatformInfo,
                        (cl_platform_id   platform,
                         cl_platform_info param_name,
                         size_t           param_value_size,
                         void *           param_value,
                         size_t *         param_value_size_ret),
                        (platform,
                         param_name,
                         param_value_size,
                         param_value,
                         param_value_size_ret),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clGetDeviceIDs,
                        (cl_platform_id platform,
                         cl_device_type device_type,
                         cl_uint        num_entries,
                         cl_device_id * devices,
                         cl_uint *      num_devices),
                        (platform,
                         device_type,
                         num_entries,
                         devices,
                         num_devices),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clGetDeviceInfo,
                        (cl_device_id   device,
                         cl_device_info param_name,
                         size_t         param_value_size,
                         void *         param_value,
                         size_t *       param_value_size_ret),
                        (device,
                         param_name,
                         param_value_size,
                         param_value,
                         param_value_size_ret),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clRetainDevice,
                        (cl_device_id device),
                        (device),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clReleaseDevice,
                        (cl_device_id device),
                        (device),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_HANDLE_WRAPPER(
    cl_context,
    clCreateContext,
    (const cl_context_properties *properties,
     cl_uint                      num_devices,
     const cl_device_id          *devices,
     void (CL_CALLBACK *pfn_notify)(const char *errinfo,
                                    const void *private_info,
                                    size_t      cb,
                                    void       *user_data),
     void                        *user_data,
     cl_int                      *errcode_ret),
    (properties, num_devices, devices, pfn_notify, user_data, errcode_ret),
    errcode_ret)

PDJE_OPENCL_HANDLE_WRAPPER(
    cl_context,
    clCreateContextFromType,
    (const cl_context_properties *properties,
     cl_device_type               device_type,
     void (CL_CALLBACK *pfn_notify)(const char *errinfo,
                                    const void *private_info,
                                    size_t      cb,
                                    void       *user_data),
     void                        *user_data,
     cl_int                      *errcode_ret),
    (properties, device_type, pfn_notify, user_data, errcode_ret),
    errcode_ret)

PDJE_OPENCL_INT_WRAPPER(clGetContextInfo,
                        (cl_context      context,
                         cl_context_info param_name,
                         size_t          param_value_size,
                         void *          param_value,
                         size_t *        param_value_size_ret),
                        (context,
                         param_name,
                         param_value_size,
                         param_value,
                         param_value_size_ret),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clRetainContext,
                        (cl_context context),
                        (context),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clReleaseContext,
                        (cl_context context),
                        (context),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_HANDLE_WRAPPER(
    cl_command_queue,
    clCreateCommandQueue,
    (cl_context                     context,
     cl_device_id                   device,
     cl_command_queue_properties    properties,
     cl_int                        *errcode_ret),
    (context, device, properties, errcode_ret),
    errcode_ret)

PDJE_OPENCL_HANDLE_WRAPPER(
    cl_command_queue,
    clCreateCommandQueueWithProperties,
    (cl_context                context,
     cl_device_id              device,
     const cl_queue_properties *properties,
     cl_int                   *errcode_ret),
    (context, device, properties, errcode_ret),
    errcode_ret)

PDJE_OPENCL_INT_WRAPPER(clGetCommandQueueInfo,
                        (cl_command_queue      command_queue,
                         cl_command_queue_info param_name,
                         size_t                param_value_size,
                         void *                param_value,
                         size_t *              param_value_size_ret),
                        (command_queue,
                         param_name,
                         param_value_size,
                         param_value,
                         param_value_size_ret),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clRetainCommandQueue,
                        (cl_command_queue command_queue),
                        (command_queue),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clReleaseCommandQueue,
                        (cl_command_queue command_queue),
                        (command_queue),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clFlush,
                        (cl_command_queue command_queue),
                        (command_queue),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clFinish,
                        (cl_command_queue command_queue),
                        (command_queue),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_HANDLE_WRAPPER(
    cl_program,
    clCreateProgramWithSource,
    (cl_context         context,
     cl_uint            count,
     const char       **strings,
     const size_t      *lengths,
     cl_int            *errcode_ret),
    (context, count, strings, lengths, errcode_ret),
    errcode_ret)

PDJE_OPENCL_INT_WRAPPER(
    clBuildProgram,
    (cl_program           program,
     cl_uint              num_devices,
     const cl_device_id  *device_list,
     const char          *options,
     void (CL_CALLBACK *pfn_notify)(cl_program program, void *user_data),
     void                *user_data),
    (program, num_devices, device_list, options, pfn_notify, user_data),
    GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clGetProgramInfo,
                        (cl_program      program,
                         cl_program_info param_name,
                         size_t          param_value_size,
                         void *          param_value,
                         size_t *        param_value_size_ret),
                        (program,
                         param_name,
                         param_value_size,
                         param_value,
                         param_value_size_ret),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(
    clGetProgramBuildInfo,
    (cl_program            program,
     cl_device_id          device,
     cl_program_build_info param_name,
     size_t                param_value_size,
     void                 *param_value,
     size_t               *param_value_size_ret),
    (program,
     device,
     param_name,
     param_value_size,
     param_value,
     param_value_size_ret),
    GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clRetainProgram,
                        (cl_program program),
                        (program),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clReleaseProgram,
                        (cl_program program),
                        (program),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_HANDLE_WRAPPER(cl_kernel,
                           clCreateKernel,
                           (cl_program   program,
                            const char  *kernel_name,
                            cl_int      *errcode_ret),
                           (program, kernel_name, errcode_ret),
                           errcode_ret)

PDJE_OPENCL_INT_WRAPPER(clSetKernelArg,
                        (cl_kernel    kernel,
                         cl_uint      arg_index,
                         size_t       arg_size,
                         const void * arg_value),
                        (kernel, arg_index, arg_size, arg_value),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clGetKernelInfo,
                        (cl_kernel       kernel,
                         cl_kernel_info  param_name,
                         size_t          param_value_size,
                         void *          param_value,
                         size_t *        param_value_size_ret),
                        (kernel,
                         param_name,
                         param_value_size,
                         param_value,
                         param_value_size_ret),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(
    clGetKernelWorkGroupInfo,
    (cl_kernel                 kernel,
     cl_device_id              device,
     cl_kernel_work_group_info param_name,
     size_t                    param_value_size,
     void                     *param_value,
     size_t                   *param_value_size_ret),
    (kernel,
     device,
     param_name,
     param_value_size,
     param_value,
     param_value_size_ret),
    GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clRetainKernel,
                        (cl_kernel kernel),
                        (kernel),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clReleaseKernel,
                        (cl_kernel kernel),
                        (kernel),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_HANDLE_WRAPPER(cl_mem,
                           clCreateBuffer,
                           (cl_context  context,
                            cl_mem_flags flags,
                            size_t       size,
                            void        *host_ptr,
                            cl_int      *errcode_ret),
                           (context, flags, size, host_ptr, errcode_ret),
                           errcode_ret)

PDJE_OPENCL_HANDLE_WRAPPER(
    cl_mem,
    clCreateBufferWithProperties,
    (cl_context                   context,
     const cl_mem_properties     *properties,
     cl_mem_flags                 flags,
     size_t                       size,
     void                        *host_ptr,
     cl_int                      *errcode_ret),
    (context, properties, flags, size, host_ptr, errcode_ret),
    errcode_ret)

PDJE_OPENCL_INT_WRAPPER(clGetMemObjectInfo,
                        (cl_mem       memobj,
                         cl_mem_info  param_name,
                         size_t       param_value_size,
                         void *       param_value,
                         size_t *     param_value_size_ret),
                        (memobj,
                         param_name,
                         param_value_size,
                         param_value,
                         param_value_size_ret),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clRetainMemObject,
                        (cl_mem memobj),
                        (memobj),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clReleaseMemObject,
                        (cl_mem memobj),
                        (memobj),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clEnqueueReadBuffer,
                        (cl_command_queue command_queue,
                         cl_mem           buffer,
                         cl_bool          blocking_read,
                         size_t           offset,
                         size_t           size,
                         void *           ptr,
                         cl_uint          num_events_in_wait_list,
                         const cl_event * event_wait_list,
                         cl_event *       event),
                        (command_queue,
                         buffer,
                         blocking_read,
                         offset,
                         size,
                         ptr,
                         num_events_in_wait_list,
                         event_wait_list,
                         event),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clEnqueueWriteBuffer,
                        (cl_command_queue command_queue,
                         cl_mem           buffer,
                         cl_bool          blocking_write,
                         size_t           offset,
                         size_t           size,
                         const void *     ptr,
                         cl_uint          num_events_in_wait_list,
                         const cl_event * event_wait_list,
                         cl_event *       event),
                        (command_queue,
                         buffer,
                         blocking_write,
                         offset,
                         size,
                         ptr,
                         num_events_in_wait_list,
                         event_wait_list,
                         event),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clEnqueueNDRangeKernel,
                        (cl_command_queue command_queue,
                         cl_kernel        kernel,
                         cl_uint          work_dim,
                         const size_t *   global_work_offset,
                         const size_t *   global_work_size,
                         const size_t *   local_work_size,
                         cl_uint          num_events_in_wait_list,
                         const cl_event * event_wait_list,
                         cl_event *       event),
                        (command_queue,
                         kernel,
                         work_dim,
                         global_work_offset,
                         global_work_size,
                         local_work_size,
                         num_events_in_wait_list,
                         event_wait_list,
                         event),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clWaitForEvents,
                        (cl_uint          num_events,
                         const cl_event * event_list),
                        (num_events, event_list),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clGetEventInfo,
                        (cl_event      event,
                         cl_event_info param_name,
                         size_t        param_value_size,
                         void *        param_value,
                         size_t *      param_value_size_ret),
                        (event,
                         param_name,
                         param_value_size,
                         param_value,
                         param_value_size_ret),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clRetainEvent,
                        (cl_event event),
                        (event),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_INT_WRAPPER(clReleaseEvent,
                        (cl_event event),
                        (event),
                        GetUnavailableOpenCLError())

PDJE_OPENCL_VOIDPTR_WRAPPER(clGetExtensionFunctionAddress,
                            (const char * func_name),
                            (func_name))

PDJE_OPENCL_VOIDPTR_WRAPPER(clGetExtensionFunctionAddressForPlatform,
                            (cl_platform_id platform,
                             const char *   func_name),
                            (platform, func_name))

#undef PDJE_OPENCL_INT_WRAPPER
#undef PDJE_OPENCL_HANDLE_WRAPPER
#undef PDJE_OPENCL_VOIDPTR_WRAPPER
#undef PDJE_OPENCL_RUNTIME_SYMBOLS

} // namespace

namespace PDJE_PARALLEL {

bool
EnsureOpenCLRuntimeLoaded() noexcept
{
#if !defined(_WIN32) && !defined(__linux__)
    return false;
#else
    std::call_once(gOpenCLRuntimeInitOnce, InitializeOpenCLRuntime);
    return gOpenCLRuntimeReady.load(std::memory_order_acquire);
#endif
}

} // namespace PDJE_PARALLEL
