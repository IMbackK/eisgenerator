#pragma once
#include <complex>
#include <string>
#include <vector>
#include "componant.h"

namespace eis
{

class Inductor: public Componant
{
public:
	Inductor(std::string paramStr, size_t count = 10);
	Inductor(fvalue L = 1e-6);
	virtual std::complex<fvalue> execute(fvalue omega) override;
	virtual size_t paramCount() override;
	virtual char getComponantChar() const override;
	static constexpr char staticGetComponantChar(){return 'l';}
	virtual ~Inductor() = default;
};

}
