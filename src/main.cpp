// std includes
#include <cstdlib>
#include <iostream>
#include <string>
// dpp includes
#include <dpp/dpp.h>
// logger includes
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <fmt/format.h>
#include <iomanip>

using namespace std;
int main()
{
	// get the token from the environment variable
	const char *env_p = std::getenv("TOKEN_BOT_ASTRA");

	if (env_p == nullptr)
	{
		std::cerr << "The environment variable TOKEN_BOT_ASTRA is not set." << std::endl;
		std::cerr << "Please set it to the token of the bot in the environment variables." << std::endl;
		std::cerr << "Exiting..." << std::endl;
		return 1;
	}

	// transform the token to string from char*
	std::string token(env_p);

	dpp::cluster bot(token);

	const std::string log_name = "Astra.log";

	std::shared_ptr<spdlog::logger> log;
	spdlog::init_thread_pool(8192, 2);
	std::vector<spdlog::sink_ptr> sinks;
	auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_name, 1024 * 1024 * 5, 10);
	sinks.push_back(stdout_sink);
	sinks.push_back(rotating);
	log = std::make_shared<spdlog::async_logger>("logs", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
	spdlog::register_logger(log);
	log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L] [th#%t]%$ : %v");
	log->set_level(spdlog::level::level_enum::debug);

	/* Integrate spdlog logger to D++ log events */
	bot.on_log([&bot, &log](const dpp::log_t &event)
			   {
	        switch (event.severity) {
	            case dpp::ll_trace:
	                log->trace("{}", event.message);
	            break;
	            case dpp::ll_debug:
	                log->debug("{}", event.message);
	            break;
	            case dpp::ll_info:
	                log->info("{}", event.message);
	            break;
	            case dpp::ll_warning:
	                log->warn("{}", event.message);
	            break;
	            case dpp::ll_error:
	                log->error("{}", event.message);
	            break;
	            case dpp::ll_critical:
	            default:
	                log->critical("{}", event.message);
	            break;
	        } });

	bot.on_slashcommand([](const dpp::slashcommand_t &event)
						{
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        } });

	bot.on_slashcommand([&bot](const dpp::slashcommand_t &event)
						{
	        /* Check which command they ran */
	        if (event.command.get_command_name() == "embed") {
	            /* Create an embed */
	            dpp::embed embed = dpp::embed()
	                .set_color(dpp::colors::sti_blue)
	                .set_title("Some name")
	                .set_url("https://dpp.dev/")
	                .set_author("Some name", "https://dpp.dev/", "https://dpp.dev/DPP-Logo.png")
	                .set_description("Some description here")
	                .set_thumbnail("https://dpp.dev/DPP-Logo.png")
	                .add_field(
	                    "Regular field title",
	                    "Some value here"
	                )
	                .add_field(
	                    "Inline field title",
	                    "Some value here",
	                    true
	                )
	                .add_field(
	                    "Inline field title",
	                    "Some value here",
	                    true
	                )
	                .set_image("https://dpp.dev/DPP-Logo.png")
	                .set_footer(
	                    dpp::embed_footer()
	                    .set_text("Some footer text here")
	                    .set_icon("https://dpp.dev/DPP-Logo.png")
	                )
	                .set_timestamp(time(0));
	 
	            /* Create a message with the content as our new embed. */
	            dpp::message msg(event.command.channel_id, embed);
	 
	            /* Reply to the user with the message, containing our embed. */
	            event.reply(msg);
	        } });

	bot.on_ready([&bot](const dpp::ready_t &event)
				 {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("embed", "Send an embed!", bot.me.id));
        } });

	bot.start(dpp::st_wait);
}
