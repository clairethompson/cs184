#include "BRDF.h"
#include "color.h"

BRDF::BRDF(Color a, Color d, Color s, float sp, Color r) {
	this->ka = a;
	this->kd = d;
	this->ks = s;
	this->sp = sp;
	this->kr = r;
}

BRDF::BRDF() : ka(Color()), kd(Color()), ks(Color()), sp(0.0), kr(Color()) {}

Color BRDF::getKA() { return this->ka; }
Color BRDF::getKD() { return this->kd; }
Color BRDF::getKS() { return this->ks; }
Color BRDF::getKR() { return this->kr; }
float BRDF::getSP() { return this->sp; }