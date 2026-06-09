////-------------------------------------------------------------------------
////
//// 로그
//// 
//// spdlog 라이브러리 래핑
//// c/c++ - command line -> /utf-8 추가
//// 
//// 사용예시
//// SPDLOG_INFO("hello! {} {}", 1, 2);
//// SPDLOG_ERROR("hello! {} {}", 1, 2);
////
////-------------------------------------------------------------------------
//#include <memory>
//#include <vector>
//
//#include <spdlog/spdlog.h>
//#include <spdlog/async.h>
//
//#include <spdlog/sinks/basic_file_sink.h>
//#include <spdlog/sinks/stdout_color_sinks.h>
//
//#include "Singleton.h"
//
//class Log : public Singleton<Log>
//{
//public:
//    ~Log()
//    {
//        spdlog::shutdown();
//    }
//
//    void initialize()
//    {
//        spdlog::init_thread_pool(8192, 1);
//
//        auto file_sink =  std::make_shared<spdlog::sinks::basic_file_sink_mt>("Log/server.txt",false);
//
//        auto console_sink =   std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//
//        std::vector<spdlog::sink_ptr> sinks
//        {
//            // 파일 출력
//            file_sink,
//
//            // 콘솔 출력
//            console_sink
//        };
//
//        auto logger =
//            std::make_shared<spdlog::async_logger>(
//                "server",
//                sinks.begin(),
//                sinks.end(),
//                spdlog::thread_pool(),
//                spdlog::async_overflow_policy::block);
//
//        //logger->set_pattern(
//        //    "[%Y-%m-%d %H:%M:%S.%e] [T%t] [%^%l%$] %v");
//
//        logger->set_pattern(
//            "[%Y-%m-%d %H:%M:%S.%e] "
//            "[T%t] "
//            "[%^%l%$] "
//            "[%s:%# %!] "
//            "%v");
//
//        logger->set_level(spdlog::level::info);
//
//        spdlog::set_default_logger(logger);
//
//        spdlog::flush_every(std::chrono::seconds(3));
//
//        spdlog::flush_on(spdlog::level::err);
//    }
//};
