#include <stdio.h>
#include "../src/carrito.h"
#include "minunit/minunit.h"

/*
 * Tests de integracion: verifican que las funciones trabajan bien
 * en combinacion, no de forma aislada.
 */

/* ═══════════════════════════════════════════════════════════════════════════
 *  PARTE D — Escribir el test guiado (ver README.md, Parte 8)
 * ═══════════════════════════════════════════════════════════════════════════ */
void test_compra_con_descuento(void) {
    printf("\n[compra con descuento]\n");
    Carrito c;
    carrito_init(&c);
    Producto pan   = {"Pan",   200, 3};   /* subtotal: 600 */
    Producto leche = {"Leche", 350, 2};   /* subtotal: 700 */
    carrito_agregar(&c, pan);
    carrito_agregar(&c, leche);
    ASSERT_IGUAL(1300, carrito_total(&c));
    ASSERT_IGUAL(1170, carrito_descuento(carrito_total(&c), 10));  /* 1300 - 10% = 1170 */
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  PARTE E — Disenar un test propio (ver README.md, Parte 9)
 * ═══════════════════════════════════════════════════════════════════════════ */

void test_agregar_hasta_llenar(void) {
    printf("\n[llenar el carrito hasta el maximo]\n");
    Carrito c;
    carrito_init(&c);
    Producto p = {"Leche", 350, 1};
    carrito_agregar(&c, p);
    carrito_agregar(&c, p);
    carrito_agregar(&c, p);
    carrito_agregar(&c, p);
    ASSERT_IGUAL(MAX_ITEMS, carrito_contar(&c));
    ASSERT_IGUAL(0, carrito_agregar(&c, p));       /* el 5to debe fallar */
    ASSERT_IGUAL(MAX_ITEMS, carrito_contar(&c));   /* el conteo no cambio */
}

int main(void) {
    printf("=== Tests de integracion ===");
    
    test_compra_con_descuento();  
     test_agregar_hasta_llenar();  
    RESUMEN();
    return EXIT_CODE();
}
