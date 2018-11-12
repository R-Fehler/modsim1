#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// In dieser Funktion werden die Wertepaare abgezählt,
// indem die entsprechenden Zeilen gezählt werden.
//
// Die Ausgabe dieser Funktion ist gleich der Anzahl der Wertepaare.
int getNumberOfPoints(char *name) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;

  if ((fp = fopen(name, "r")) == NULL) {
    exit(EXIT_FAILURE);
  }

  int cnt = 0;
  while (getline(&line, &len, fp) != -1) {
    cnt++;
  }

  free(line);

  return cnt;
}

// In dieser Funktion werden die Wertepaare eingelesen und
// in Form von Arrays x[N] und y[N] übergeben.
void readFile(char *name, double x[], double y[]) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;

  if ((fp = fopen(name, "r")) == NULL) {
    exit(EXIT_FAILURE);
  }

  int cnt = 0;
  while (getline(&line, &len, fp) != -1) {
    sscanf(line, "%lf %lf", &x[cnt], &y[cnt]);
    cnt++;
  }

  free(line);
}

int main(int argc, char *argv[]) {
  // Abzählen der Wertepaare.
  int N = getNumberOfPoints("input.dat");

  double x[N];  // Vektor für die Abstände der Messungen
  double y[N];  // Vektor für die gemessenen Werte

  // Einlesen der Daten.
  readFile("input.dat", x, y);

  // Laufvariablen
  int i, j;
  double D[N][N];  // für eine Matrix der Dimnsion NxN

  // Berechnen Sie die dividierten Differenzen:
  for (int i = 0; i < N; i++) {
    D[i][0] = y[i];
  }

  for (int i = 1; i < N; i++) {
    for (int j = 1; j <= i; j++) {
      D[i][j] = (D[i][j - 1] - D[i - 1][j - 1]) / (x[i] - x[i - j]);
    }
  }
  // Geben Sie hier die dividierten Differenzen in Form einer Matrix aus:
  printf("\n  Ausgabe der dividierten Differenzen D: \n");

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%8.5lf", D[i][j]);
    }
    printf("\n");
  }

  /*********************************************************************/
  /* Zu Erinnerung:                                                    */
  /* %3d     - Ganzzahl mit 3 Stellen                                  */
  /* %.14lf  - Fließkommazahl mit 14 Nachkommastellen                  */
  /* \n      - Neue Zeile                                              */
  /* Beispiel: printf("%3d: % .2lf % .2lf\n", Variablen);              */
  /*          000: -0.01  0.01                                         */
  /*********************************************************************/

  // Weisen Sie dem Vektor a die Polynomkoeffizienten aus den entsprechenden
  // Einträgen der Matrix D zu:
  double a[N];  // Vektor der Länge N
  for (int i = 0; i < N; i++) {
    a[i] = D[i][i];
  }
  // Geben Sie das Polynom aus:
  printf("%.3lf", a[0]);
  for (int i = 1; i < N; i++) {
    printf("%.3lf", a[i]);
    for (size_t j = 0; j < i; j++) {
      printf("(x-%.1lf", x[j]);
      printf(")");
    }
    printf("\n");
  }
  printf("\n");

  int useplotter = 1;

  if (useplotter) {
    // Plotten des Polynoms
    FILE *gp = popen("gnuplot -p", "w");
    fprintf(gp,
            "set key right bottom box; set xrange [0:5]; set yrange [-8:3.5]; "
            "set xlabel \"x\"; set ylabel \"y\";\n");
    fprintf(gp, "f(x) = ");
    for (i = 0; i < N; i++) {
      fprintf(gp, "+%lf", a[i]);
      for (j = 0; j < i; j++) {
        fprintf(gp, "*(x-%lf)", x[j]);
      }
    }
    fprintf(gp, ";\n");
    fprintf(gp,
            "plot f(x) lc 3 ti \"Interpolation\", \"input.dat\" lc 4 ps 3 lw 2 "
            "ti \"data\";\n");

    pclose(gp);
  }

  return 0;
}
