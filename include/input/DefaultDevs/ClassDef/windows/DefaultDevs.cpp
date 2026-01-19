#include "DefaultDevs.hpp"
#include "NameGen.hpp"
#include "PDJE_RAII_WRAP.hpp"
#include "ipc_util.hpp"

namespace PDJE_DEFAULT_DEVICES {

bool
DefaultDevs::OpenProcess(const fs::path &pt)
{
    start_up_info    = STARTUPINFOW{};
    process_info     = PROCESS_INFORMATION{};
    start_up_info.cb = sizeof(start_up_info);
    try {
        subprocess_RAII.val = CreateJobObjectW(nullptr, nullptr);
        if (subprocess_RAII.get() == INVALID_HANDLE_VALUE) {
            throw std::runtime_error(
                "failed to run CreateJobObjectW. Errcode: " +
                std::to_string(GetLastError()));
        }
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION sub_raii_info{};
        sub_raii_info.BasicLimitInformation.LimitFlags =
            JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

        if (!SetInformationJobObject(subprocess_RAII.get(),
                                     JobObjectExtendedLimitInformation,
                                     &sub_raii_info,
                                     sizeof(sub_raii_info))) {
            throw std::runtime_error(
                "failed to run SetInformationJobObject. Errcode: " +
                std::to_string(GetLastError()));
        }
        WINRAII file_locker;
        file_locker.val =
            CreateFileW(pt.wstring().c_str(),
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        nullptr,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                        nullptr);
        if (!file_locker.get()) {
            critlog("failed to lock subprocess exe file");
            return false;
        }
        if (!PDJE_IPC::HashCompare(pt)) {
            critlog("hash not matched. maybe Under Attack.");
            return false;
        }
        WINRAII readHdl;
        readHdl.val              = pipe.Gen();
        start_up_info.dwFlags    = STARTF_USESTDHANDLES;
        start_up_info.hStdInput  = readHdl.get();
        start_up_info.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        start_up_info.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
        auto cmd                 = pt.wstring();
        BOOL ok                  = CreateProcessW(nullptr,
                                 cmd.data(),
                                 nullptr,
                                 nullptr,
                                 TRUE,
                                 CREATE_NO_WINDOW | CREATE_SUSPENDED,
                                 nullptr,
                                 nullptr,
                                 &start_up_info,
                                 &process_info);
        if (!ok) {
            critlog("failed to create child process. Err:");
            critlog(GetLastError());
            return false;
        }

        if (!AssignProcessToJobObject(subprocess_RAII.get(),
                                      process_info.hProcess)) {
            TerminateProcess(process_info.hProcess, 1);
            throw std::runtime_error("AssignProcessToJobObject failed: " +
                                     std::to_string(GetLastError()));
        }
        ResumeThread(process_info.hThread);
        CloseHandle(process_info.hThread);
        process_info.hThread = nullptr;

    } catch (const std::exception &e) {
        if (process_info.hProcess) {
            CloseHandle(process_info.hProcess);
        }
        if (process_info.hThread) {
            CloseHandle(process_info.hThread);
        }
        critlog("exception on creating child process. Err:");
        critlog(e.what());
        return false;
    }
    return true;
}

DefaultDevs::DefaultDevs()
{

    auto path = GetValidProcessExecutor();

    if (!OpenProcess(path)) {
        critlog("failed to open child process. Err:");
        critlog(GetLastError());
        return;
    }
    pipe.Send(meta.GenTXRX().str());
    meta.Listen();
    if (!meta.QueryHealth()) {
        critlog("Query Health Failed on MainProcess init.");
    }
}

void
DefaultDevs::InitEvents()
{
    auto namegen  = PDJE_IPC::RANDOM_GEN();
    auto loop_run = namegen.Gen("PDJE_IPC_EVENT_LOOP_RUN_");
    auto term     = namegen.Gen("PDJE_IPC_EVENT_TERMINATE_");

    events.input_loop_run_event.HostInit(loop_run);
    events.terminate_event.HostInit(term);
    meta.SendIPCSharedMemory(1, loop_run, "EVENT_input_loop_run");
    meta.SendIPCSharedMemory(1, term, "EVENT_terminate");
}

void
DefaultDevs::Ready()
{
    PDJE_IPC::Input_Transfer_Metadata cfg;
    PDJE_IPC::RANDOM_GEN              rg;
    cfg.max_length              = 2048;
    cfg.lenname                 = rg.Gen("PDJE_INPUT_LEN_");
    cfg.bodyname                = rg.Gen("PDJE_INPUT_BODY_");
    cfg.hmacname                = rg.Gen("PDJE_INPUT_HMAC_");
    cfg.data_request_event_name = rg.Gen("PDJE_INPUT_REQ_EVENT_");
    cfg.data_stored_event_name  = rg.Gen("PDJE_INPUT_STORED_EVENT_");
    input_buffer.emplace(cfg);
    meta.SendInputTransfer(input_buffer.value());
    InitEvents();
}

bool
DefaultDevs::Config(const std::vector<DeviceData> &devs)
{
    nlohmann::json nj;
    try {

        nj["body"] = nlohmann::json::array();
        for (const auto &dev : devs) {
            std::unordered_map<std::string, std::string> kv;
            kv["id"]   = dev.device_specific_id;
            kv["name"] = dev.Name;
            switch (dev.Type) {
            case PDJE_Dev_Type::KEYBOARD:
                kv["type"] = "KEYBOARD";
                nj["body"].push_back(kv);
                break;
            case PDJE_Dev_Type::MOUSE:
                kv["type"] = "MOUSE";
                nj["body"].push_back(kv);
                break;

            default:
                break;
            }
        }
    } catch (const std::exception &e) {
        critlog("failed to make json device configure query. What: ");
        critlog(e.what());
        return false;
    }
    return meta.QueryConfig(nj.dump());
}
DefaultDevs::~DefaultDevs()
{
    if (process_info.hProcess) {
        CloseHandle(process_info.hProcess);
    }
    if (process_info.hThread) {
        CloseHandle(process_info.hThread);
    }
}
}; // namespace PDJE_DEFAULT_DEVICES