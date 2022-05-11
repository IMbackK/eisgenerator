#pragma once
#include <string>
#include <vector>
#include <argp.h>
#include "model.h"
#include "log.h"
#include "tokenize.h"

const char *argp_program_version = "eisgenerator-1.0";
const char *argp_program_bug_address = "<carl@uvos.xyz>";
static char doc[] = "Application that generates EIS spectra from model strings";
static char args_doc[] = "";

static struct argp_option options[] =
{
  {"verbose",   'v', 0,      0,  "Show debug messages" },
  {"quiet",     'q', 0,      0,  "only output data" },
  {"param",     'p', 0,      0,  "select parameter sweep mode" },
  {"model",      'm', "[STRING]",    0,  "set model string" },
  {"omega",      'o', "[START-END]", 0,  "set omega range" },
  {"omegasteps", 'c', "[COUNT]",     0,  "set omega range steps" },
  {"log",       'l', 0,      0,  "use logarithmic steps" },
  {"normalize", 'n', 0,      0,  "normalize values" },
  {"reduce",    'r', 0,      0,  "reduce values to \"interesting\" range" },
  {"hz",        'h', 0,      0,  "freqency values as temporal frequency instead of angular frequency"},
  {"invert",        'i', 0,      0,  "inverts the imaginary axis"},
  { 0 }
};

enum
{
	MODE_SWEEP = 0,
	MODE_PARAM_SWEEP
};

struct Config
{
	std::string modelStr = "c{1e-6}r{1e3}-r{1e3}";
	unsigned int mode = MODE_SWEEP;
	eis::Range omegaRange;
	bool normalize = false;
	bool reduce = false;
	bool hertz = false;
	bool invert = false;

	Config(): omegaRange(1, 1001, 1)
	{}
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
	Config *config = reinterpret_cast<Config*>(state->input);

	switch (key)
	{
	case 'q':
		eis::Log::level = eis::Log::ERROR;
		break;
	case 'v':
		eis::Log::level = eis::Log::DEBUG;
		break;
	case 's':
		config->mode = MODE_SWEEP;
		break;
	case 'p':
		config->mode = MODE_PARAM_SWEEP;
		break;
	case 'm':
		config->modelStr.assign(arg);
		break;
	case 'l':
		config->omegaRange.log = true;
		break;
	case 'n':
		config->normalize = true;
		break;
	case 'r':
		config->reduce = true;
		break;
	case 'h':
		config->hertz = true;
		break;
	case 'i':
		config->invert = true;
		break;
	case 'c':
		try
		{
			config->omegaRange.count = static_cast<size_t>(std::stoul(std::string(arg)));
		}
		catch(const std::invalid_argument& ia)
		{
			argp_usage(state);
		}
		break;
	case 'o':
	{
		std::vector<std::string> tokens = tokenize(std::string(arg), '-');
		if(tokens.size() != 2)
		{
			argp_usage(state);
			break;
		}
		try
		{
			double start = std::stod(tokens[0]);
			double end = std::stod(tokens[1]);
			if(start < end)
			{
				config->omegaRange.start = start;
				config->omegaRange.end = end;
			}
			else
			{
				config->omegaRange.start = end;
				config->omegaRange.end = start;
			}
		}
		catch(const std::invalid_argument& ia)
		{
			argp_usage(state);
		}
		break;
	}
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};