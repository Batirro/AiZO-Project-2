

Algorytmy i Złożoność Obliczeniowa
Projekt 2 – (21 kwietnia 2026)
mgr inż. Damian Mroziński
Zakres projektu
C/C++, algorytmy grafowe, analiza wyników, praca na plikach, skryptowanie
Jakie są cele projektu?
•Zapoznanie z różnymi algorytmami grafowymi, ich implementacja oraz analiza efektywności.
•Nabycie umiejętności dopasowania programu do wymagań / istniejących klas.
•Nauczenie się podstawowego skryptowania.
•Mądre planowanie projektu, aby nie komplikować sobie życia.
•Korzystanie z gotowych datasetów oraz ich szukanie.
## TL;DR
Bardzo podobny projekt do poprzedniego. Samodzielna implementacja grafów w różnej reprezen-
tacji,  do  tego  algorytmy  dla  kilku  problemów  i  analiza  wyników.  Nie  można  używać  gotowych
rozwiązań, również w zakresie struktur danych (typustd::vector,std::list). Jeśli takie rozwią-
zania są potrzebne, trzeba je stworzyć samemu, od zera, z ręcznym zarządzaniem pamięcią.
Założenia projektu
-  Wszystkie  użyte  struktury  danych  powinny  być  zaimplementowane  ręcznie  oraz  alokowane
i zwalniane dynamicznie (zgodnie z badanym rozmiarem tablicy).
-  Obowiązuje C++ w wersji 17.
-  Do kompilacji należy przygotować plik cmake’a. Należy wyróżnić lokalizację z kodem, z bi-
bliotekami oraz z nagłówkami. Dołączanie lokalizacji to robota cmake’a. Nie należy skakać po
lokalizacjach przy pisaniu#include ...
-  Program musi kompilować siębez ostrzeżen(tak, ostrzeżeń!).
Wystarczy dodać podstawowy zestaw flag do kompilatora
-Wall -Wextra -Werror.
## 1

-  Program nie może zostać przerwany błędem. Używanietry..catchdo obsługi błędów typo-
wych/przewidywalnych nie jest dopuszczalne.
-  Pojedynczy  pomiar  jest  niemiarodajny.  Aby  badania  zostały  wykonane  prawidłowo,  nale-
ży wywołać każdy przypadek wielokrotnie (np 50 razy), a wyniki uśrednić. Należy również
sprawdzić minimum oraz maksimum.
-  Wszystkie wyniki przeprowadzonych badań należy zachować, najprościej dopisywać je jako
kolejne  wiersze  pliku  .csv,  zapisując  zarówno  datę  i  godzinę  wykonania  badania,  wszystkie
parametry programu oraz sam czas.
-  Czas  badania  należy  zmierzyć  w  mikrosekundach  za  pomocąstd::chrono.  Mierzony  czas
dotyczy wyłącznie sortowania i nie wlicza się do niego ładowanie danych z pliku, losowanie
wartości, czy zapis wyników do pliku.
-  Należy zachować spójność badań, aby można było sensownie porównywać wyniki.
-  Kod musi być sformatowany spójnie i zawierać komentarze.
-  Program powinien wykorzystywać szablony, aby łatwo można było wykorzystać zaimplemen-
towane algorytmy do grafów w różnych reprezentacjach.
NAPRAWDĘ WARTO!Wymagane na 5.0, ale przyda się każdemu.
-  Plik z danymi wejściowymi zawsze jest zbudowany w ten sam sposób.
(a)  W pierwszej linii znajduje się liczba wierzchołków oraz liczba krawędzi (rozdzielone ta-
bulatorem, w tej kolejności).
(b)  W liniach 2-... znajduje się opis krawędzi (każda w osobnej linii). Wartości są rozdzielone
tabulatorem. W następującej kolejności: wierzchołek początkowy, wierzchołek końcowy
oraz waga/przepustowość.
(c)  Wierzchołki numerowane są w sposób ciągły, od zera.
(d)  Dla  problemu  MST  pojedyncza  krawędź  jest  krawędzią  nieskierowaną,  natomiast  dla
algorytmów najkrótszej drogi i maksymalnego przepływu jest krawędzią skierowaną.
(e)  Zakładamy, że użytkownik poda poprawny plik (skierowany/nieskierowany) w zależności
od problemu.
Poniższy przykład to graf z czterema wierzchołkami i sześcioma krawędziami.
## 4   6
## 0   1   10
## 0   2   10
## 1   3   5
## 2   3   12
## 3   0   1
## 3   2   7
## 2

Warto zauważyć, że gdyby nie ostatnia krawędź to ten plik
mógłby być również plikiem określającym graf nieskierowany
-  Domyślnie waga/koszt/przepustowość jest liczbą naturalną.
-  Wygenerowane  grafy  muszą  być  grafami  spójnymi.  Można  najpierw  wygenerować  drzewo
rozpinające, a następnie losować kolejne krawędzie aż do uzyskania wymaganej gęstości (we-
ryfikacja poprawności).
-  Należy przeprowadzić weryfikację poprawności na przykład wczytanych wartości. Czy liczba
krawędzi w stworzonej i załadowanej strukturze zgadza się z danymi wejściowymi?
-  Program musi móc pokazać graf (w obu formach), a także rozwiązanie zadanego problemu,
czyli na przykład najkrótszą ścieżkę razem z jej kosztem. Brak ustandaryzowanego sposobu
wyświetlania  w  pliku  wyjściowym.  Należy  pamiętać  aby  było  jasne  co  jest  wypisywane  na
ekranie (tj. liczba bez kontekstu nie jest wystarczająca).
-  Zakres wag powinien zależeć od liczby krawędzi (k) w grafie, np. zakresrand(1,k*4/5), aby
umożliwić istnienie krawędzi o tej samej wadze.
-  Program powinien posiadać trzy tryby działania.
a)Tryb pojedynczego testu, gdzie następuje załadowanie grafu do zaimplementowanej
struktury/reprezentacji oraz rozwiązanie zadanego problemu z użyciem tej struktury/re-
prezentacji.
b)Tryb badań, gdzie parametry określają środowisko (m.in. wybrany algorytm, plik wyj-
ściowy z wynikami, liczba powtórzeń, reprezentacje...).
c)Tryb pomocy, gdzie wyświetlana jest instrukcja użycia programu.
Jeśli, z jakiegoś powodu jest potrzeba innych trybów,
proszę je zaimplementować i opisać w helpie.
-  Sterowanie programem odbywa się przy użyciu argumentów do metody głównej(Typ pracy,
plik  wejściowy,  algorytm,  gęstość  grafu  i  tak  dalej). Do ładowania parametrów należy użyć
biblioteki udostępnionej przez prowadzącego.NIE można używać innych parametrów,
jeśli czegoś brakuje, proszę założyć issue w repozytorium.
Sprawdzanie wyników algorytmów będzie częściowo odbywać się
skryptami, stąd ścisłe wymagania do obsługi parametrów wejściowych.
-  Repozytorium z biblioteką do parametrów oraz przykładowym testowym użyciem.
https://gitlab.com/mrozo94/for-students
## 3

-  Z racji na złożoność czasową losowania grafów i aby móc łatwo testować algorytmy na tych
samych danych bez konieczności zapisywania ich na dysku,będzie możliwość wybrania
opcji która będzie reprezentować uruchomienie wszystkich algorytmów dla dane-
go problemu i/lub wszystkich reprezentacji.
-  Środowisko,  którego  będę  używał  do  sprawdzania  prac  i  na  którym  powinno  się  wszystko
kompilować:
•Kompilator gcc 15.2.0(paczka gotowa przez apt).
•System automatycznego budowania cmake 3.31.6(paczka gotowa przez apt).
•System Xubuntu (25.10) wirtualizowany za pomocą VirtualBox (7.2.4) -tak  naprawdę
to akurat nie ma takiego znaczenia.
Sposoby reprezentacji grafu:
-  reprezentacja macierzowa (macierz incydencji),
-  reprezentacja listowa (lista następników/poprzedników).
Analizowane problemy
Problem MST- Wyznaczanie minimalnego drzewa rozpinającego dla grafu nieskierowanego:
-  algorytm Prima
-  algorytm Kruskala
Problem SP- Wyznaczanie najkrótszej ścieżki w grafie dla grafu skierowanego:
-  algorytm Dijkstry
-  algorytm Forda-Bellmana
Problem MF- Wyznaczanie maksymalnego przepływu:
-  algorytm Forda-Fulkersona
Opisy badań
Badanie A- Wpływ rozmiaru grafu (liczby wierzchołków).
Badanie polega na sprawdzeniu jak algorytmy oraz reprezentacje działają na grafie o gęstości
50%? Należy przetestować sześć różnych rozmiarów grafów.
Badanie B- Wpływ gęstości grafu (stosunku liczby krawędzi do analogicznej dla grafu pełnego).
Badanie polega na sprawdzeniu jak algorytmy oraz reprezentacje działają na grafach o wy-
branym rozmiarze i gęstościach 25,50,75,99%.
## 4

Ocenianie - wymagane algorytmy i dodatkowe zadania
Ocena wyjściowa 3.0
•Wszystkie wymienione badania (zakresy).
•Obie reprezentacje grafów. Po jednym algorytmie dla MST oraz SP.
•Pozostałe:
Obiektowość NIE jest wymagana.
Repozytorium NIE jest wymagane.
Szablony NIE są wymagane.
Ocena wyjściowa 4.0
•Wszystkie wymienione badania (zakresy).
•Obie reprezentacje grafów. Po dwa algorytmy dla MST oraz SP.
•Pozostałe:
Obiektowość JEST wymagana.
Repozytorium JEST wymagane.
Szablony NIE są wymagane.
Ocena wyjściowa 5.0
•Wszystkie wymienione badania(mniejsze zakresy: pięć [A] oraz trzy [B]).
•Obie reprezentacje grafów. Po dwa algorytmy dla MST oraz SP. Jeden algorytm dla MF.
•Dodatkowo należy przeprowadzić badania na dużym wybranym datasecie znalezionym
w Internecie i porównać na nim swoje implementacje z dowolnym gotowym rozwiązanie-
m/narzędziem/implementacją. Do datasetów proponuje zerknąć na, chociażby
https://snap.stanford.edu/data/index.html.
W sytuacji gdy dataset nie będzie nam pasował pod jakimś względem
(nie mówię o konieczności dodania na przykład liczby węzłów do
pierwszej lini, to można zrobić zawsze, bo dopasowujemy plik
wejściowy do naszych potrzeb) i naprawdę nie możemy znaleźć innego,
to można go zmodyfikować, opisując te zmiany w sprawozdaniu.
•Pozostałe:
Obiektowość JEST wymagana.
Repozytorium JEST wymagane.
Szablony SĄ wymagane.
JEŚLI KTOŚ MA INNY POMYSŁ,  może  chciałby  sprawdzić  inny  algorytm  czy  inne
zadanie. Przetestować coś inaczej, skupić się bardziej na datasetach i porównaniu z większą liczbą
gotowych rozwiązań, może zrobić testy z kolegą z grupy? Może przeprowadzić bardziej szczegółowe
badania zamiast czegoś innego. Za zgodą prowadzącego można coś zmienić.
W razie pytań, pozostaję do dyspozycji.
## 5
