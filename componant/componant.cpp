//
// eisgenerator - a shared libary and application to generate EIS spectra
// Copyright (C) 2022-2024 Carl Philipp Klemm <carl@uvos.xyz>
//
// This file is part of eisgenerator.
//
// eisgenerator is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// eisgenerator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with eisgenerator.  If not, see <http://www.gnu.org/licenses/>.
//

#include "componant/componant.h"
#include <assert.h>
#include <sstream>
#include "componant/paralellseriel.h"
#include "componant/resistor.h"
#include "componant/cap.h"
#include "componant/constantphase.h"
#include "componant/trc.h"
#include "componant/warburg.h"
#include "log.h"
#include "componant/inductor.h"
#include "componant/tro.h"
#include "componant/trc.h"
#include "randomgen.h"

using namespace eis;

void Componant::setParamRanges(const std::vector<eis::Range>& rangesIn)
{
	ranges = rangesIn;
}

std::vector<eis::Range>& Componant::getParamRanges()
{
	return ranges;
}

std::vector<eis::Range> Componant::getParamRanges() const
{
	return ranges;
}

std::vector<eis::Range> Componant::getDefaultParameters(bool range) const
{
	(void)range;
	assert(paramCount() == 0);
	return std::vector<eis::Range>();
}

std::string Componant::getComponantString(bool currentValue) const
{
	std::stringstream out;
	out<<getComponantChar();
	std::vector<eis::Range> ranges = getParamRanges();

	if(ranges.empty())
		return out.str();

	out<<'{';
	for(const eis::Range& range : ranges)
	{
		if(currentValue)
			out<<range.stepValue()<<", ";
		else
			out<<range.getString()<<", ";
	}

	std::string outStr = out.str();
	outStr.pop_back();
	outStr.back() = '}';
	return outStr;
}

std::string Componant::getUniqueName()
{
	if(uniqueName.empty())
	{
		uniqueName.push_back(getComponantChar());
		for(size_t i = 0; i < 3; ++i)
		{
			char ch = static_cast<char>(rd::rand(122-65)+65);
			if(ch > 90 && ch < 97)
				ch = ch + 6;
			uniqueName.push_back(ch);
		}
	}
	return uniqueName;
}

bool Componant::compileable()
{
	std::vector<std::string> parameters;
	return !getCode(parameters).empty();
}

std::string Componant::getCode(std::vector<std::string>& parameters)
{
	(void)parameters;
	return std::string();
}

std::string Componant::getTorchScript(std::vector<std::string>& parameters)
{
	(void)parameters;
	return std::string();
}

Componant* Componant::copy(Componant* componant)
{
	switch(componant->getComponantChar())
	{
		case Resistor::staticGetComponantChar():
			return new Resistor(*dynamic_cast<Resistor*>(componant));
		case Cap::staticGetComponantChar():
			return new Cap(*dynamic_cast<Cap*>(componant));
		case Inductor::staticGetComponantChar():
			return new Inductor(*dynamic_cast<Inductor*>(componant));
		case Cpe::staticGetComponantChar():
			return new Cpe(*dynamic_cast<Cpe*>(componant));
		case Warburg::staticGetComponantChar():
			return new Warburg(*dynamic_cast<Warburg*>(componant));
		case TransmissionLineOpen::staticGetComponantChar():
			return new TransmissionLineOpen(*dynamic_cast<TransmissionLineOpen*>(componant));
		case TransmissionLineClosed::staticGetComponantChar():
			return new TransmissionLineClosed(*dynamic_cast<TransmissionLineClosed*>(componant));
		case Parallel::staticGetComponantChar():
			return new Parallel(*dynamic_cast<Parallel*>(componant));
		case Serial::staticGetComponantChar():
			return new Serial(*dynamic_cast<Serial*>(componant));
		default:
			Log(Log::ERROR)<<"unimplmented type copy for "<<componant->getComponantChar();
			assert(0);
			break;
	}
	return nullptr;
}

Componant* Componant::createNewComponant(char componant, std::string paramStr, size_t count, bool defaultToRange)
{
	switch(componant)
	{
		case Cap::staticGetComponantChar():
			return new Cap(paramStr, count, defaultToRange);
		case Resistor::staticGetComponantChar():
			return new Resistor(paramStr, count, defaultToRange);
		case Inductor::staticGetComponantChar():
			return new Inductor(paramStr, count, defaultToRange);
		case Cpe::staticGetComponantChar():
			return new Cpe(paramStr, count, defaultToRange);
		case Warburg::staticGetComponantChar():
			return new Warburg(paramStr, count, defaultToRange);
		case TransmissionLineOpen::staticGetComponantChar():
			return new TransmissionLineOpen(paramStr, count, defaultToRange);
		case TransmissionLineClosed::staticGetComponantChar():
			return new TransmissionLineClosed(paramStr, count, defaultToRange);
		default:
			return nullptr;
	}
}

bool Componant::isValidComponantChar(char componantCh)
{
	switch(componantCh)
	{
		case Cap::staticGetComponantChar():
		case Resistor::staticGetComponantChar():
		case Inductor::staticGetComponantChar():
		case Cpe::staticGetComponantChar():
		case Warburg::staticGetComponantChar():
		case TransmissionLineOpen::staticGetComponantChar():
		case TransmissionLineClosed::staticGetComponantChar():
			return true;
		default:
			return false;
	}
}
