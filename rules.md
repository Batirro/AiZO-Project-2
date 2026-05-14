# Cel projektu
Celem projektu jest samodzielna implementacja wybranych algorytmów grafowych w języku C++17 (bez użycia gotowych kontenerów biblioteki standardowej takich jak `std::vector` czy `std::list`), analiza ich złożoności i efektywności, zaimplementowanie własnych struktur danych z dynamicznym zarządzaniem pamięcią oraz poprawne zorganizowanie całego środowiska i struktury projektu.

# Zasady i wytyczne dla Agenta

## 1. Śledzenie postępów (Todo list)
- **ZASADA BEZWZGLĘDNA**: Wszystkie zrealizowane zadania, zaimplementowane funkcjonalności i wykonane etapy prac muszą być na bieżąco dokumentowane i dopisywane pod nagłówkiem `## Todo list` w odpowiednim pliku lub notatkach roboczych, aby zachować ciągłość pracy i przejrzystość postępów.

## 2. Ograniczenia techniczne i organizacja kodu
- **Język i kompilacja**: Używamy C++17. Projekt musi być budowany za pomocą CMake. Drzewo projektu musi rozdzielać kod źródłowy, nagłówki i biblioteki zewnętrzne. Użycie dyrektyw `#include` ma polegać na konfiguracji CMake, a nie relatywnym skakaniu po katalogach (np. `../src/`).
- **Jakość kodu**: Kod musi kompilować się bez jakichkolwiek ostrzeżeń (należy użyć flag `-Wall -Wextra -Werror`).
- **Struktury danych**: ZAKAZ używania gotowych kontenerów C++ typu `std::vector`, `std::list`. Wszystkie struktury należy zaimplementować ręcznie od zera, wykorzystując szablony (templates), programowanie obiektowe (OOP) oraz ręczną dynamiczną alokację/dealokację pamięci zależną od potrzeb.
- **Błędy**: ZAKAZ używania `try..catch` do przewidywalnych błędów. Program nie może zamykać się z błędem krytycznym (crash).

## 3. Reprezentacja grafów i ich generowanie
- **Dwie reprezentacje**: 
  1. Macierzowa (Macierz incydencji).
  2. Listowa (Lista następników/poprzedników).
- **Spójność**: Wygenerowane grafy muszą być spójne (zaleca się najpierw generować drzewo rozpinające, a następnie losować brakujące krawędzie).
- **Optymalne losowanie**: Przy losowaniu grafów o dużych gęstościach losowanie krawędzi powinno odbywać się w sposób optymalny (np. poprzez odejmowanie wylosowanych krawędzi od maksymalnej puli możliwych dla grafu pełnego).
- **Gęstości**: Należy poprawnie i dokładnie zaokrąglać liczby wyliczając ilość krawędzi dla docelowych gęstości.
- **Wagi krawędzi**: Domyślnie wagi/koszty to liczby naturalne. Zakres losowanych wag zależy od liczby krawędzi $k$, np. zakres `rand(1, k * 4/5)`, aby wymusić istnienie krawędzi o takich samych wagach.

## 4. Analizowane problemy i algorytmy
- **Problem MST (Minimalne drzewo rozpinające)** - dla grafu nieskierowanego:
  - Algorytm Prima.
  - Algorytm Kruskala.
- **Problem SP (Najkrótsza ścieżka)** - dla grafu skierowanego:
  - Algorytm Dijkstry.
  - Algorytm Forda-Bellmana.
- **Problem MF (Maksymalny przepływ)** - dla grafu skierowanego:
  - Algorytm Forda-Fulkersona.

## 5. Dane wejściowe, wyjściowe i UI
- **Pliki wejściowe**: 
  - Linia 1: `liczba_wierzchołków \t liczba_krawędzi`.
  - Linie 2+: `wierzchołek_początkowy \t wierzchołek_końcowy \t waga_lub_przepustowość`.
  - Wierzchołki są indeksowane od zera. Weryfikuj poprawność wczytanych danych (np. czy zgadza się zadeklarowana liczba krawędzi).
- **Wyświetlanie**: Program musi potrafić czytelnie zaprezentować graf w obu reprezentacjach na ekranie oraz wynik algorytmu (np. trasę najkrótszej ścieżki i jej łączny koszt). Użytkownik musi wiedzieć, co widzi.
- **Tryby działania (własny help)**:
  1. Tryb pojedynczego testu (wczytanie i rozwiązanie z użyciem konkretnej reprezentacji).
  2. Tryb badań (automatyczne uruchamianie pomiarów na podstawie środowiska).
  3. Tryb pomocy (własny, czytelny Help z opisem parametrów).
  4. Dodatkowy tryb „uruchom wszystko” dla łatwego testowania wszystkich algorytmów/reprezentacji na wylosowanym grafie w pamięci, aby pominąć narzut dysku.
- **Parametry CLI**: Do obsługi argumentów należy bezwzględnie używać biblioteki zaleconej przez prowadzącego (https://gitlab.com/mrozo94/for-students).

## 6. Zbieranie wyników i pomiary
- **Pomiary czasu**: Czas należy mierzyć wyłącznie dla wykonania samego algorytmu, pomijając alokacje początkowe, wczytywanie plików czy zapis. Używamy `std::chrono` z dokładnością do mikrosekund.
- **Liczba powtórzeń**: Każdy przypadek badawczy należy wykonać wielokrotnie (np. 50 razy).
- **Zapis wyników**: 
  - Wyniki zapisujemy do plików `.csv`. 
  - Zapisujemy **wszystkie dane z pomiarów** (każdy z osobna), a nie tylko uśredniony wynik. Najlepiej zapisywać wszystkie pomiary do jednego lub kilku jasnych, spójnych plików.
  - W rekordzie CSV muszą być zawarte: data, godzina, wszystkie parametry programu oraz zmierzony czas.
- **Opisy Badań do przeprowadzenia**:
  - **Badanie A**: Wpływ rozmiaru (6 różnych rozmiarów wierzchołków) na grafach o gęstości stałej = 50%.
  - **Badanie B**: Wpływ gęstości (25%, 50%, 75%, 99%) dla konkretnego rozmiaru grafu.

## Todo list
*(Tutaj będą dopisywane zrealizowane i planowane etapy prac)*
