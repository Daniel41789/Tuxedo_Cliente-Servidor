// Programa del servidor
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <atmi.h>
#include <userlog.h>
#include <fml32.h>
#include "biblioOper.fml.h"

int tpsvrinit(int argc, char* argv[])
{
    userlog("Iniciando srvTuxedo");
    return 0;
}

int raiz(int numero) {
    if (numero < 0) {
        printf("No se puede calcular la raíz cuadrada de un número negativo.\n");
        return 0.0;
    }

    float estimacion = numero / 2.0; 
    int i;
    for (i = 0; i < 10; ++i) {
        estimacion = 0.5 * (estimacion + numero / estimacion);
    }

    return estimacion;
}

// Servicio para calcular el área de un círculo
void svcCalcAreaCirculo(TPSVCINFO *rqst)
{
    float fvL_operaUno; 
    float fvL_operaDos;
    float fvL_operaTres;
    float fvL_operaCuatro;
    float fvL_resulOpe;
    float fvL_resulOpeDos;
    FBFR32* fbfr = (FBFR32*)rqst->data;

    userlog("Servicio svcCalcAreaCirculo Invocado.");

    // Obtenemos el radio del círculo
    if (Fget32(fbfr, OPERUN, 0, (char*)&fvL_operaUno, 0) < 0)
    {
        userlog("No se leyo el radio del círculo");
        tpreturn(TPFAIL, 0, (char *)fbfr, 0L, 0);
    }

    // Calculamos el área del círculo
    fvL_resulOpe = 3.1415 * fvL_operaUno * fvL_operaUno;

    // Enviamos el resultado del área
    Fadd32(fbfr, RESUL, (char*)&fvL_resulOpe, 0);

    userlog("Area del círculo: [%f]", fvL_resulOpe);

    tpreturn(TPSUCCESS, 0, rqst->data, 0L, 0);
}

// Servicio para calcular las raíces de una ecuación de segundo grado
void svcCalcEcSegGrado(TPSVCINFO *rqst)
{
    float fvL_operaUno; 
    float fvL_operaDos;
    float fvL_operaTres;
    float fvL_operaCuatro;
    float fvL_resulOpe;
    float fvL_resulOpeDos;
    FBFR32* fbfr = (FBFR32*)rqst->data;

    userlog("Servicio svcCalcEcSegGrado Invocado.");

    // Obtenemos los coeficientes de la ecuación de segundo grado
    if (Fget32(fbfr, OPERUN, 0, (char*)&fvL_operaUno, 0) < 0)
    {
        userlog("No se leyo el coeficiente a");
        tpreturn(TPFAIL, 0, (char *) fbfr, 0L, 0);
    }
    if (Fget32(fbfr, OPERDO, 0, (char*)&fvL_operaDos, 0) < 0)
    {
        userlog("No se leyo el coeficiente b");
        tpreturn(TPFAIL, 0, (char *) fbfr, 0L, 0);
    }
    if (Fget32(fbfr, OPERTR, 0, (char*)&fvL_operaTres, 0) < 0)
    {
        userlog("No se leyo el coeficiente c");
        tpreturn(TPFAIL, 0, (char *) fbfr, 0L, 0);
    }

    // Calculamos las raíces de la ecuación de segundo grado
    fvL_resulOpe = ((-1*fvL_operaDos) + raiz(fvL_operaDos * fvL_operaDos - 4 * fvL_operaUno * fvL_operaTres)) / (2 * fvL_operaUno);
    fvL_resulOpeDos = ((-1*fvL_operaDos) - raiz(fvL_operaDos * fvL_operaDos - 4 * fvL_operaUno * fvL_operaTres)) / (2 * fvL_operaUno);

    // Enviamos las raíces de la ecuación de segundo grado
    Fadd32(fbfr, RESUL, (char*)&fvL_resulOpe , 0);
    Fadd32(fbfr, RESULD, (char*)&fvL_resulOpeDos, 0);

    userlog("Raíz 1: %.0f  Raíz 2: %.0f", fvL_resulOpe, fvL_resulOpeDos);

    tpreturn(TPSUCCESS, 0, rqst->data, 0L, 0);
}

// Servicio para calcular el producto punto de dos vectores en 2D
void servCalcProdPunto(TPSVCINFO *rqst)
{
    float fvL_operaUno; 
    float fvL_operaDos;
    float fvL_operaTres;
    float fvL_operaCuatro;
    float fvL_resulOpe;
    float fvL_resulOpeDos;
    FBFR32* fbfr = (FBFR32*)rqst->data;

    userlog("Servicio servCalcProdPunto Invocado.");

    // Obtenemos las componentes de los vectores A y B
    if (Fget32(fbfr, OPERUN, 0, (char*)&fvL_operaUno, 0) < 0) {
        userlog("No se leyó la componente x del vector A");
        tpreturn(TPFAIL, 0, (char *) fbfr, 0L, 0);
    }
    if (Fget32(fbfr, OPERDO, 0, (char*)&fvL_operaDos, 0) < 0) {
        userlog("No se leyó la componente y del vector A");
        tpreturn(TPFAIL, 0, (char *) fbfr, 0L, 0);
    }
    if (Fget32(fbfr, OPERTR, 0, (char*)&fvL_operaTres, 0) < 0) {
        userlog("No se leyó la componente x del vector B");
        tpreturn(TPFAIL, 0, (char *) fbfr, 0L, 0);
    }
    if (Fget32(fbfr, OPERCU, 0, (char*)&fvL_operaCuatro, 0) < 0) {
        userlog("No se leyó la componente y del vector B");
        tpreturn(TPFAIL, 0, (char *) fbfr, 0L, 0);
    }

    // Calculamos el producto punto
    fvL_resulOpe = ((fvL_operaUno * fvL_operaTres) + (fvL_operaDos * fvL_operaCuatro));

    // Enviamos el resultado del producto punto
    Fadd32(fbfr, RESUL, (char*)&fvL_resulOpe, 0);

    userlog("Producto punto: [%f]", fvL_resulOpe);

    tpreturn(TPSUCCESS, 0, rqst->data, 0L, 0);
}