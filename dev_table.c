/*
  stm32flash - Open Source ST STM32 flash program for *nix
  Copyright (C) 2010 Geoffrey McRae <geoff@spacevs.com>
  Copyright (C) 2014-2015 Antonio Borneo <borneo.antonio@gmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "stm32.h"

/*
 * Device table, corresponds to the "Bootloader device-dependant parameters"
 * table in ST document AN2606.
 * Note that the option bytes upper range is inclusive!
 */
const stm32_dev_t devices[] = {
	/* F0 */
	{0x440, "STM32F030x8/F05xxx"              , 0x20000800, 0x20002000, 0x08000000, 0x08010000,  4, 1024, 0x1FFFF800, 0x1FFFF80F, 0x1FFFEC00, 0x1FFFF800},
	{0x442, "STM32F030xC/F09xxx"              , 0x20001800, 0x20008000, 0x08000000, 0x08040000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFC800, 0x1FFFF800},
	{0x444, "STM32F03xx4/6"                   , 0x20000800, 0x20001000, 0x08000000, 0x08008000,  4, 1024, 0x1FFFF800, 0x1FFFF80F, 0x1FFFEC00, 0x1FFFF800},
	{0x445, "STM32F04xxx/F070x6"              , 0x20001800, 0x20001800, 0x08000000, 0x08008000,  4, 1024, 0x1FFFF800, 0x1FFFF80F, 0x1FFFC400, 0x1FFFF800},
	{0x448, "STM32F070xB/F071xx/F72xx"        , 0x20001800, 0x20004000, 0x08000000, 0x08020000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFC800, 0x1FFFF800},
	/* F1 */
	{0x412, "STM32F10xxx Low-density"         , 0x20000200, 0x20002800, 0x08000000, 0x08008000,  4, 1024, 0x1FFFF800, 0x1FFFF80F, 0x1FFFF000, 0x1FFFF800},
	{0x410, "STM32F10xxx Medium-density"      , 0x20000200, 0x20005000, 0x08000000, 0x08020000,  4, 1024, 0x1FFFF800, 0x1FFFF80F, 0x1FFFF000, 0x1FFFF800},
	{0x414, "STM32F10xxx High-density"        , 0x20000200, 0x20010000, 0x08000000, 0x08080000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFF000, 0x1FFFF800},
	{0x420, "STM32F10xxx Medium-density VL"   , 0x20000200, 0x20002000, 0x08000000, 0x08020000,  4, 1024, 0x1FFFF800, 0x1FFFF80F, 0x1FFFF000, 0x1FFFF800},
	{0x428, "STM32F10xxx High-density VL"     , 0x20000200, 0x20008000, 0x08000000, 0x08080000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFF000, 0x1FFFF800},
	{0x418, "STM32F105xx/F107xx"              , 0x20001000, 0x20010000, 0x08000000, 0x08040000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFB000, 0x1FFFF800},
	{0x430, "STM32F10xxx XL-density"          , 0x20000800, 0x20018000, 0x08000000, 0x08100000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFE000, 0x1FFFF800},
	/* Note that F2 and F4 devices have sectors of different page sizes
           and only the first sectors (of one page size) are included here */
	/* F2 */
	{0x411, "STM32F2xxxx"                     , 0x20002000, 0x20020000, 0x08000000, 0x08100000,  4, 16384, 0x1FFFC000, 0x1FFFC00F, 0x1FFF0000, 0x1FFF7800},
	/* F3 */
	{0x432, "STM32F373xx/F378xx"              , 0x20001400, 0x20008000, 0x08000000, 0x08040000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFD800, 0x1FFFF800},
	{0x422, "STM32F302xB(C)/F303xB(C)/F358xx" , 0x20001400, 0x2000A000, 0x08000000, 0x08040000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFD800, 0x1FFFF800},
	{0x439, "STM32F301xx/F302x4(6/8)/F318xx"  , 0x20001800, 0x20004000, 0x08000000, 0x08010000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFD800, 0x1FFFF800},
	{0x438, "STM32F303x4(6/8)/F334xx/F328xx"  , 0x20001800, 0x20003000, 0x08000000, 0x08010000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFD800, 0x1FFFF800},
	{0x446, "STM32F302xD(E)/F303xD(E)/F398xx" , 0x20001800, 0x20010000, 0x08000000, 0x08080000,  2, 2048, 0x1FFFF800, 0x1FFFF80F, 0x1FFFD800, 0x1FFFF800},
	/* F4 */
	{0x413, "STM32F40xxx/41xxx"               , 0x20003000, 0x20020000, 0x08000000, 0x08100000,  4, 16384, 0x1FFFC000, 0x1FFFC00F, 0x1FFF0000, 0x1FFF7800},
	{0x419, "STM32F42xxx/43xxx"               , 0x20003000, 0x20030000, 0x08000000, 0x08200000,  4, 16384, 0x1FFEC000, 0x1FFFC00F, 0x1FFF0000, 0x1FFF7800},
	{0x423, "STM32F401xB(C)"                  , 0x20003000, 0x20010000, 0x08000000, 0x08040000,  4, 16384, 0x1FFFC000, 0x1FFFC00F, 0x1FFF0000, 0x1FFF7800},
	{0x433, "STM32F401xD(E)"                  , 0x20003000, 0x20018000, 0x08000000, 0x08080000,  4, 16384, 0x1FFFC000, 0x1FFFC00F, 0x1FFF0000, 0x1FFF7800},
	{0x431, "STM32F411xx"                     , 0x20003000, 0x20020000, 0x08000000, 0x08080000,  4, 16384, 0x1FFFC000, 0x1FFFC007, 0x1FFF0000, 0x1FFF7A10},
	{0x421, "STM32F446xx"                     , 0x20003000, 0x20020000, 0x08000000, 0x08200000,  4, 16384, 0x1FFFC000, 0x1FFFC00F, 0x1FFF0000, 0x1FFF7A10},
	/* L0 */
	{0x417, "L05xxx/06xxx"                    , 0x20001000, 0x20002000, 0x08000000, 0x08010000, 32,  128, 0x1FF80000, 0x1FF8000F, 0x1FF00000, 0x1FF01000},
	/* L1 */
	{0x416, "L1xxx6(8/B)"                     , 0x20000800, 0x20004000, 0x08000000, 0x08020000, 16,  256, 0x1FF80000, 0x1FF8000F, 0x1FF00000, 0x1FF01000},
	{0x429, "L1xxx6(8/B)A"                    , 0x20001000, 0x20008000, 0x08000000, 0x08020000, 16,  256, 0x1FF80000, 0x1FF8000F, 0x1FF00000, 0x1FF01000},
	{0x427, "L1xxxC"                          , 0x20001000, 0x20008000, 0x08000000, 0x08020000, 16,  256, 0x1FF80000, 0x1FF8000F, 0x1FF00000, 0x1FF02000},
	{0x436, "L1xxxD"                          , 0x20001000, 0x2000C000, 0x08000000, 0x08060000, 16,  256, 0x1ff80000, 0x1ff8000F, 0x1FF00000, 0x1FF02000},
	{0x437, "L1xxxE"                          , 0x20001000, 0x20014000, 0x08000000, 0x08060000, 16,  256, 0x1ff80000, 0x1ff8000F, 0x1FF00000, 0x1FF02000},
	/* These are not (yet) in AN2606: */
	{0x641, "Medium_Density PL"               , 0x20000200, 0x20005000, 0x08000000, 0x08020000,  4, 1024, 0x1FFFF800, 0x1FFFF80F, 0x1FFFF000, 0x1FFFF800},
	{0x9a8, "STM32W-128K"                     , 0x20000200, 0x20002000, 0x08000000, 0x08020000,  1, 1024, 0, 0, 0, 0},
	{0x9b0, "STM32W-256K"                     , 0x20000200, 0x20004000, 0x08000000, 0x08040000,  1, 2048, 0, 0, 0, 0},
	{0x0}
};
