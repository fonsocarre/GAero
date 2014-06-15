define(angle, 3)dnl
define(Umag, 100)dnl
define(turbK, 0.004056)dnl
define(turbOmega, 0.254748)dnl
define(p, 0.)dnl
dnl
changecom(//)changequote([,])dnl
define(calc, [esyscmd(perl -e 'printf ($1)')])dnl
define(PI, 3.141592)dnl
dnl
define(sinn, calc(sin(angle*3.1416/180)))dnl
define(coss, calc(cos(angle*3.1416/180)))dnl
define(uVel, calc(Umag*coss))dnl
define(vVel, calc(Umag*sinn))dnl
/*--------------------------------*- C++ -*----------------------------------*\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  2.3.0                                 |
|   \\  /    A nd           | Web:      www.OpenFOAM.org                      |
|    \\/     M anipulation  |                                                 |
\*---------------------------------------------------------------------------*/

velocityMag	Umag;
flowVelocity	(uVel 0 vVel);
pressure	p;
turbulentKE	turbK;
turbulentOmega	turbOmega;
liftVector	(-sinn 0 coss);
dragVector	(coss 0 sinn);
pitchVector	(0 1 0);

#inputMode           merge
/*****************************************************************************/
