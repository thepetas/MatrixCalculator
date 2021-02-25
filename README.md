# MatrixCalculator

This project was created as school project for course BI-PA2, CVUT FIT.

All about copyright is in the LICENCE file.

For documentation run `make doc`. - dependence on Doxygen
For program run `make compile` - dependence on g++ compiler

Or run simply `make all`


* Source code - src directory
* Documentation - doc directory

Assignment:
```
Maticová kalkulačka

Vytvořte aplikaci implementující maticovou kalkulačku.

Kalkulačka musí implementovat:

1. práci s maticemi různé velikosti
2. základní operace: sčítání, odčítání, násobení, transpozice
3. sloučení (3x3 sloučeno s 3x3 => matice 6x3) a oříznutí (např. pouze levá polovina matice 6x3 => levá 3x3)
4. pro čtvertcové matice navíc výpočet inverze
5. efektivní paměťovou reprezentaci (pro řídké matice se nehodí 2D pole, pro husté matice naopak)
6. funkce pro výpočet determinantu, určení hodnosti, Gaussova eliminační metoda
7. proměnné (uložení matice do proměnné, načítání matice ze vstupu, využití proměnné ve výpočtu)

Použití polymorfismu (doporučeně)

typ matice: proměnná, řídká (mapa, list) vs. hustá (2D pole)
operace: sčítání, odčítání, násobení, ...
varianta GEMu: klasická, s komentářem
Ukázka běhu programu (není závazná):

SCAN X[3][3]
Y[3][3] = 1 // jednotkova matice
Z = MERGE X Y
GEM Z -v // s detaily
X = SPLIT Z [3][3] (3, 0) // z matice Z, chci jen 3x3 od pozice (3, 0)
PRINT X
```

