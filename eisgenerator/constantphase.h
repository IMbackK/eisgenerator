#pragma once
#include <complex>
#include <string>
#include <vector>
#include "componant.h"

namespace eis
{

class Cpe: public Componant
{
private:
	void setDefaultParam();
public:
	Cpe(std::string paramStr, size_t count = 10);
	Cpe(fvalue q, fvalue alpha);
	Cpe();
	virtual std::complex<fvalue> execute(fvalue omega) override;
	virtual size_t paramCount() override;
	virtual char getComponantChar() const override;
	static constexpr char staticGetComponantChar(){return 'p';}
	virtual ~Cpe() = default;

};

}
