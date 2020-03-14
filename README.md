# Programowanie wielowątkowe

Nauka wykorzystania wielu wątków w programie na przykładzie podstawowych operacji macierzowych.

## Spis treści

- [Programowanie wielowątkowe](#programowanie-wielowątkowe)
  - [Spis treści](#spis-treści)
  - [Cel zadania](#cel-zadania)
  - [Przebieg działania programu](#przebieg-działania-programu)
  - [Kryteria](#kryteria)
  - [Rozwiązanie zadania](#rozwiązanie-zadania)
    - [Implementacja w C](#implementacja-w-c)
      - [Uruchomienie](#uruchomienie)

## Cel zadania

Napisać program, który:

- wykonuje mnożenie dwóch macierzy lub (wg decyzji prowadzącego),
- oblicza sumę elementów macierzy wyjściowej,
- wyznacza [normę Frobeniusa](http://en.wikipedia.org/wiki/Matrix_norm#Frobenius_norm) wyjściowej (pierwiastek sumy kwadratów wszystkich elementów macierzy).

## Przebieg działania programu

Program wczytuje z plików dwie macierze, a następnie wykonuje ich mnożenie. Mnożenie powinno przebiegać współbieżnie i ma być wykonywane przez N utworzonych w programie wątków. Podział obliczeń najwydajniej jest przeprowadzić według macierzy wynikowej. Należy unikać niepotrzebnej alokacji i kopiowania pamięci (każdy wątek operuje na tych samych danych (używając wskaźników) i zapisuje wyniki do współdzielonego miejsca w pamięci. Każdy z wątków powinien ponadto obliczać sumę elementów wyliczonego fragmentu macierzy i dodawać ją do zmiennej globalnej przechowującej całkowitą sumę.

1. Wczytanie macierzy z pliku oraz inicjalizacja macierzy wynikowej.
2. Stworzenie indeksów określających podział danych obliczeniowych
3. Utworzenie N wątków potomnych
4. Obliczenie cząstkowych wyników mnożenia przez poszczególne wątki.
5. Obliczenie w poszczególnych wątkach sumy elementów fragmentów macierzy i zapisanie ich do zmiennej globalnej.
6. Zebranie statusów zakończenia wątków.
7. Wypisanie wyniku na ekran. (macierzy wynikowej oraz sumy elementów)
8. Zwolnienie pamięci.

## Kryteria

W ramach zaliczenia proszę napisać **dwa programy** realizujące powyższe zadanie.  
Jeden w języku C wykorzystujący wątki POSIX (pthread) a drugi w języku Java lub Python.

## Rozwiązanie zadania

Przygotowałem dwa programy realizujące równoległe mnożenie macierzy i jednoczesne sumowanie elementów macierzy wynikowej. Jedna implementacja jest napisana w C a druga w Javie.

### Implementacja w C

W celu ułatwienia obsługi programu nie przyjmuje on argumentów. Po uruchomieniu wczytuje macierze z plików _A.txt_ oraz _B.txt_. Następnie oblicza macierz wynikową oraz sumę elementów. Program wypisuje macierze A, B i macierz wynikową oraz sumę jej elementów na standardowe wyjście.

Program korzysta z wątków ze standardu POSIX.

#### Uruchomienie

Program wymaga bibliotek systemowych dostępnych na systemach Linux i Unix. **Nie będzie działać na systemie Windows**.

Do budowy programu można wykorzystać narzędzie _makefile_ i cel _release_, który jest celem domyślnym. W tym celu należy przejść do katalogu C i uruchomić następujące komendy.

```bash
./make #Budowa programu
./matrixOperations #Uruchomienie programu
```
