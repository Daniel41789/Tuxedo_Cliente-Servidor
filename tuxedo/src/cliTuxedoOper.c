#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atmi.h>
#include <userlog.h>
#include <fml32.h>
#include "biblioOper.fml.h"

int cliCalcAreaCirculo(float fvA_operaUno, float fvA_operaDos, float fvA_operaTres, float fvA_operaCuatro, float *fvA_resulOpe, float *fvA_resulOpeDos);
int cliCalcEcSegGrado(float fvA_operaUno, float fvA_operaDos, float fvA_operaTres, float fvA_operaCuatro, float *fvA_resulOpe, float *fvA_resulOpeDos);
int cliCalcProdPunto(float fvA_operaUno, float fvA_operaDos, float fvA_operaTres, float fvA_operaCuatro, float *fvA_resulOpe, float *fvA_resulOpeDos);

typedef int (*opera)(float fvA_operaUno, float fvA_operaDos, float fvA_operaTres, float fvA_operaCuatro, float *fvA_resulOpe, float *fvA_resulOpeDos);
opera selecOperacion(intivA_operArit);

int main(int argc, char **argv)
{
    int ivL_operArit;
    float fvL_operaUno;
    float fvL_operaDos;
    float fvL_operaTres;
    float fvL_operaCuatro;
    float fvL_resulOpe;
    float fvL_resulOpeDos;
    opera svL_obtOpera;

    if (argc != 6)
    {
        printf("Modo de uso:            cliTuxedoOper OPERACION     OPER1   OPER2   OPER3   OPER4 \n");
        printf("Ejemplos:\n");
        printf("Para Area de Circulo:   cliTuxedoOper     1           5       0        0       0  \n");
        printf("Para resta:             cliTuxedoOper     2           5       3        1       0  \n");
        printf("Para multiplicacion:    cliTuxedoOper     3           5       3        2       1  \n");
        printf("NOTA: Donde hay 0 se deben de dejar 0, el resto de números si pueden cambiar \n");
        printf("NOTA 2: Los resultados son aproximados y enteros sin decimales \n");
        return 1;
    }

    ivL_operArit = atoi(argv[1]);
    fvL_operaUno = (float)atof(argv[2]);
    fvL_operaDos = (float)atof(argv[3]);
    fvL_operaTres = (float)atof(argv[4]);
    fvL_operaCuatro = (float)atof(argv[5]);

    svL_obtOpera = selecOperacion(ivL_operArit);
    svL_obtOpera(fvL_operaUno, fvL_operaDos, fvL_operaTres, fvL_operaCuatro, &fvL_resulOpe, &fvL_resulOpeDos);
    printf("Resultado de la operacion: %.0f     %.0f\n", fvL_resulOpe, fvL_resulOpeDos);
    return 0;
}

opera selecOperacion(int ivA_operArit)
{
    opera svL_obtOpera;
    switch (ivA_operArit)
    {
    case 1:
        svL_obtOpera = cliCalcAreaCirculo;
        break;
    case 2:
        svL_obtOpera = cliCalcEcSegGrado;
        break;
    case 3:
        svL_obtOpera = cliCalcProdPunto;
        break;
    default:
        svL_obtOpera = NULL;
    }
    return svL_obtOpera;
}

int cliCalcAreaCirculo(float fvA_operaUno, float fvA_operaDos, float fvA_operaTres, float fvA_operaCuatro, float *fvA_resulOpe, float *fvA_resulOpeDos)
{
    int ivL_resLlamd;
    long lvL_tamLongt;
    float fvL_recValor;
    FBFR32 *fbfr;

    /* Conectamos con la aplicacion Tuxedo Server */
    printf("Conectamos con la aplicacion\n");
    if (tpinit((TPINIT *)NULL) == -1)
    {
        printf("Error en la conexion con la aplicacion, tperrno = %d \n", tperrno);
        return (1);
    }

    // Reservamos espacio para el buffer FML
    printf("Reservamos Espacio para el buffer FML\n");
    if ((fbfr = (FBFR32 *)tpalloc("FML32", NULL, 256)) == NULL)
    {
        printf("Error Reservando espacio para Buffer FML\n");
        tpterm();
        return (1);
    }

    // Manejo del Buffer FML
    printf("Insertamos datos en buffer FML\n");
    if (Fadd32(fbfr, OPERUN, (char *)&fvA_operaUno, 0) < 0)
    {
        printf("Error insertando campo FML (OPERUN)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (1);
    }

    if (Fadd32(fbfr, OPERDO, (char *)&fvA_operaDos, 0) < 0)
    {
        printf("Error insertando campo FML (OPERDO)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    if (Fadd32(fbfr, OPERTR, (char *)&fvA_operaDos, 0) < 0)
    {
        printf("Error insertando campo FML (OPERTR)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    if (Fadd32(fbfr, OPERCU, (char *)&fvA_operaDos, 0) < 0)
    {
        printf("Error insertando campo FML (OPERCU)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    // Invocamos el servicio de suma de numeros
    printf("Llamada al servicio 'svcCalcAreaCirculo'\n");
    if (tpcall("svcCalcAreaCirculo", (char *)fbfr, 0, (char **)&fbfr, &lvL_tamLongt, 0L) == -1)
    {
        printf("Error en la llamada al servicio: tperrno = %d\n", tperrno);
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    if (Fget32(fbfr, RESUL, 0, (char *)&fvL_recValor, 0) < 0)
    {
        userlog("No se leyo el resultado\n");
        tpreturn(TPFAIL, 0, (char *)fbfr, 0L, 0);
    }

    *fvA_resulOpe = fvL_recValor;
    printf("Liberamos Buffer y desconectamos de la aplicacion\n");
    tpfree((char *)fbfr);
    tpterm();
    return 0;
}

int cliCalcEcSegGrado(float fvA_operaUno, float fvA_operaDos, float fvA_operaTres, float fvA_operaCuatro, float *fvA_resulOpe, float *fvA_resulOpeDos)
{
    int ivL_resLlamd;
    long lvL_tamLongt;
    float fvL_recValor;
    float fvL_recValor2;
    FBFR32 *fbfr;

    /* Conectamos con la aplicacion Tuxedo Server */
    printf("Conectamos con la aplicacion\n");
    if (tpinit((TPINIT *)NULL) == -1)
    {
        printf("Error en la conexion con la aplicacion, tperrno = %d \n", tperrno);
        return (1);
    }

    // Reservamos espacio para el buffer FML
    printf("Reservamos Espacio para el buffer FML\n");
    if ((fbfr = (FBFR32 *)tpalloc("FML32", NULL, 256)) == NULL)
    {
        printf("Error Reservando espacio para Buffer FML\n");
        tpterm();
        return (1);
    }

    // Manejo del Buffer FML
    printf("Insertamos datos en buffer FML\n");
    if (Fadd32(fbfr, OPERUN, (char *)&fvA_operaUno, 0) < 0)
    {
        printf("Error insertando campo FML (OPERUN)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (1);
    }

    if (Fadd32(fbfr, OPERDO, (char *)&fvA_operaDos, 0) < 0)
    {
        printf("Error insertando campo FML (OPERDO)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    if (Fadd32(fbfr, OPERTR, (char *)&fvA_operaTres, 0) < 0)
    {
        printf("Error insertando campo FML (OPERTR)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    if (Fadd32(fbfr, OPERCU, (char *)&fvA_operaCuatro, 0) < 0)
    {
        printf("Error insertando campo FML (OPERCU)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    // Invocamos el servicio de calculo de ecuación de segundo grado
    printf("Llamada al servicio 'svcCalcEcSegGrado'\n");
    if (tpcall("svcCalcEcSegGrado", (char *)fbfr, 0, (char **)&fbfr, &lvL_tamLongt, 0L) == -1)
    {
        printf("Error en la llamada al servicio: tperrno = %d\n", tperrno);
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    if (Fget32(fbfr, RESUL, 0, (char *)&fvL_recValor, 0) < 0)
    {
        userlog("No se leyo el resultado\n");
        tpreturn(TPFAIL, 0, (char *)fbfr, 0L, 0);
    }

    if (Fget32(fbfr, RESULD, 0, (char *)&fvL_recValor2, 0) < 0)
    {
        userlog("No se leyo el resultado\n");
        tpreturn(TPFAIL, 0, (char *)fbfr, 0L, 0);
    }

    *fvA_resulOpe = fvL_recValor;
    *fvA_resulOpeDos = fvL_recValor2;
    printf("Liberamos Buffer y desconectamos de la aplicacion\n");
    tpfree((char *)fbfr);
    tpterm();
    return 0;
}

int cliCalcProdPunto(float fvA_operaUno, float fvA_operaDos, float fvA_operaTres, float fvA_operaCuatro, float *fvA_resulOpe, float *fvA_resulOpeDos)
{
    int ivL_resLlamd;
    long lvL_tamLongt;
    float fvL_recValor;
    FBFR32 *fbfr;

    /* Conectamos con la aplicacion Tuxedo Server */
    printf("Conectamos con la aplicacion\n");
    if (tpinit((TPINIT *)NULL) == -1)
    {
        printf("Error en la conexion con la aplicacion, tperrno = %d \n", tperrno);
        return (1);
    }

    // Reservamos espacio para el buffer FML
    printf("Reservamos Espacio para el buffer FML\n");
    if ((fbfr = (FBFR32 *)tpalloc("FML32", NULL, 256)) == NULL)
    {
        printf("Error Reservando espacio para Buffer FML\n");
        tpterm();
        return (1);
    }

    // Manejo del Buffer FML
    printf("Insertamos datos en buffer FML\n");
    if (Fadd32(fbfr, OPERUN, (char *)&fvA_operaUno, 0) < 0)
    {
        printf("Error insertando campo FML (OPERUN)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (1);
    }

    if (Fadd32(fbfr, OPERDO, (char *)&fvA_operaDos, 0) < 0)
    {
        printf("Error insertando campo FML (OPERDO)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    if (Fadd32(fbfr, OPERTR, (char *)&fvA_operaTres, 0) < 0)
    {
        printf("Error insertando campo FML (OPERTR)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    if (Fadd32(fbfr, OPERCU, (char *)&fvA_operaCuatro, 0) < 0)
    {
        printf("Error insertando campo FML (OPERCU)\n");
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    // Invocamos el servicio de suma de numeros
    printf("Llamada al servicio 'servCalcProdPunto'\n");
    if (tpcall("servCalcProdPunto", (char *)fbfr, 0, (char **)&fbfr, &lvL_tamLongt, 0L) == -1)
    {
        printf("Error en la llamada al servicio: tperrno = %d\n", tperrno);
        printf("Liberamos Buffer y desconectamos de la aplicacion\n");
        tpfree((char *)fbfr);
        tpterm();
        return (0);
    }

    if (Fget32(fbfr, RESUL, 0, (char *)&fvL_recValor, 0) < 0)
    {
        userlog("No se leyo el resultado\n");
        tpreturn(TPFAIL, 0, (char *)fbfr, 0L, 0);
    }

    *fvA_resulOpe = fvL_recValor;
    printf("Liberamos Buffer y desconectamos de la aplicacion\n");
    tpfree((char *)fbfr);
    tpterm();
    return 0;
}