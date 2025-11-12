#include "MainProcess.hpp"
#include "PSKPipe.hpp"
#include "ipc_util.hpp"
namespace PDJE_IPC {

static bool
PDJE_OpenProcess(const fs::path       &pt,
                 Importants           &imps,
                 const int             port,
                 PDJE_CRYPTO::PSKPipe &pipe)
{
    imps.start_up_info    = STARTUPINFOW{};
    imps.process_info     = PROCESS_INFORMATION{};
    imps.start_up_info.cb = sizeof(imps.start_up_info);
    try {
        HANDLE FileLocker =
            CreateFileW(pt.wstring().c_str(),
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        nullptr,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                        nullptr);
        if (FileLocker == INVALID_HANDLE_VALUE) {
            critlog("failed to lock subprocess exe file");
            return false;
        }
        if (!PDJE_IPC::HashCompare(pt)) {
            CloseHandle(FileLocker);
            critlog("hash not matched. maybe Under Attack.");
            return false;
        }
        HANDLE readHdl                = pipe.Gen();
        imps.start_up_info.dwFlags    = STARTF_USESTDHANDLES;
        imps.start_up_info.hStdInput  = readHdl;
        imps.start_up_info.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        imps.start_up_info.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
        auto cmd                      = pt.wstring();
        BOOL ok                       = CreateProcessW(nullptr,
                                 cmd.data(),
                                 nullptr,
                                 nullptr,
                                 TRUE,
                                 CREATE_NO_WINDOW,
                                 nullptr,
                                 nullptr,
                                 &imps.start_up_info,
                                 &imps.process_info);

        CloseHandle(FileLocker);
        CloseHandle(readHdl);
        if (!ok) {
            critlog("failed to create child process. Err:");
            critlog(GetLastError());
            return false;
        }
    } catch (const std::exception &e) {
        critlog("exception on creating child process. Err:");
        critlog(e.what());
        return false;
    }
    return true;
}

MainProcess::~MainProcess()
{
    WaitForSingleObject(imp.process_info.hProcess, INFINITE);
    DWORD exitCode = 0;
    GetExitCodeProcess(imp.process_info.hProcess, &exitCode);
    if (exitCode != 0) {
        critlog("child process exit code is not zero. ErrCode: ");
        critlog(exitCode);
        return;
    }
    CloseHandle(imp.process_info.hThread);
    CloseHandle(imp.process_info.hProcess);
}

MainProcess::MainProcess(const int port)
{

    auto path = GetValidProcessExecutor();
    auto pipe = PDJE_CRYPTO::PSKPipe();

    if (!PDJE_OpenProcess(path, imp, port, pipe)) {
        critlog("failed to open child process. Err:");
        critlog(GetLastError());
        return;
    }
    if (!psk.Gen()) {
        return;
    }
    auto tokenstring = psk.Encode();
    tokenstring += " " + std::to_string(port);
    pipe.Send(tokenstring);

    aead.emplace(psk);
    cli.emplace("127.0.0.1", port);
    cli->set_connection_timeout(0, 200'000); // 200ms
    cli->set_read_timeout(0, 200'000);
    cli->set_write_timeout(0, 200'000);
    while (true) {
        if (auto res = cli->Get("/health"); res && res->status == 200) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
bool
MainProcess::EndTransmission()
{
    auto res = cli->Get("/stop");
    if (res) {
        return true;
    } else {
        return false;
    }
}

}; // namespace PDJE_IPC