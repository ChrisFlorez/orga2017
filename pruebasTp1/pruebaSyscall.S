# 05-io.S: ejemplo de entrada/salida: vamos leyendo byte por byte de la
# entrada estándar; cada byte leído, es inmediatamente enviado a stdout,
# usando SYS_write. Si ocurre un EOF en la entrada, el programa finaliza
# con código nulo; si ocurre un error de I/O, el código es no-nulo.
#
# $Date: 2009/10/23 18:59:58 $

#include <mips/regdef.h>
#include <sys/syscall.h>

    .text
    .align  2

    .globl  leer
    .ent    leer
leer:
    .frame $fp, 48, ra
    .set    noreorder
    .cpload t9
    .set    reorder
    subu    sp, sp, 48

    .cprestore 32
    sw  ra, 44(sp)
    sw  $fp, 40(sp)
    sw  gp, 36(sp)
    move    $fp, sp

while:
    # Intentamos leer un byte de información de stdin, usando
    # el stack del proceso como lugar de almacenamiento.
    #
    li  v0, SYS_read # ver dentro de <sys/syscall.h>.
    li  a0, 0        # a0: file descriptor number.
    la  a1, 28(sp)   # a1: data pointer.
    li  a2, 1        # a2: available space.
    syscall

    # Verificamos el retorno de la llamada a SYS_read: si terminó
    # bien, continuamos. De lo contrario, retornamos al sistema.
    #
    # Primero, inspeccionamos el valor del registro a3: debe ser
    # cero cuando no han ocurrido errores, y distinto de cero en
    # los demás casos.
    #
    bne a3, zero, read_error

    # En este punto, sabemos que $a3 es nulo: necesitamos revisar
    # el valor de retorno de SYS_read, i.e. cantidad de bytes que
    # se acaban de leer. Si la cantidad es exactamente 1, quiere
    # decir que tenemos leído un caracter en memoria; si es nula,
    # quiere decir que el stream ha llegado al final. Si ocurren
    # otros valores, los tratamos como error.
    #
    beq v0, zero, eof
    li  t0, 1
    bne v0, t0, read_error

    # Sólo podemos llegar aquí en el caso de leerr exactamente un
    # caracter de información, así que intentamos enviarlo por la
    # salida estándar, detectando errores. Ver más abajo.
    #
    li  v0, SYS_write # ver dentro de <sys/syscall.h>.
    li  a0, 1         # a0: file descriptor number.
    la  a1, 28(sp)    # a1: output data pointer.
    li  a2, 1         # a2: output byte size.
    syscall

    # Revisamos el retorno de SYS_write: si $a3 es no-nulo, quiere
    # decir que ha ocurrido un error. En caso contrario, imitamos
    # lo hecho en SYS_read, revisando el valor de la cantidad de
    # información que ha sido enviada.
    #
    bne a3, zero, write_error
    li  t0, 1
    bne v0, t0, write_error

    # Volvemos a iterar, intentando leer otro caracter de entrada.
    #
    b   while

write_error:
read_error:
    li  v0, SYS_exit
    li  a0, 1
    syscall

eof:
    # Finalmente, volvemos al sistema operativo devolviendo un
    # código de retorno nulo.
    #
    # Primero, restauramos los valores de los registros relevantes,
    # y luego destruímos nuestro stack frame.
    #
    move    v0, zero
    move    sp, $fp
    lw  ra, 44(sp)
    lw  $fp, 40(sp)
    addu    sp, sp, 48
    j   ra
    .end    leer
