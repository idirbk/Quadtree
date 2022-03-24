#include "quadtree.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*************************************************/
/*                                               */
/*             Implementation                    */
/*                                               */
/*************************************************/

image Construit_blanc() { return NULL; }
image Construit_noire() {
  image g = malloc(sizeof(*g));
  g->toutnoir = TRUE;
  for (int i = 0; i < 4; ++i)
    g->fils[i] = NULL;
  return g;
}

image Construit_composee(image ihg, image ihd, image ibg, image ibd) {
  image g = malloc(sizeof(*g));
  g->toutnoir = FALSE;
  image tab[4] = {ihg, ihd, ibg, ibd};
  for (int i = 0; i < 4; ++i)
    g->fils[i] = tab[i];
  return g;
}

void affiche_normal(image g) {
  if (g == NULL) {
    printf("B");
  } else {
    if (g->toutnoir) {
      printf("N");
    } else {
      printf("+");
      for (int i = 0; i < 4; ++i)
        affiche_normal(g->fils[i]);
    }
  }
}

static void affiche_profondeur_aux(image g, int p) {
  if (g == NULL) {
    printf("B%d", p);
  } else {
    if (g->toutnoir) {
      printf("N%d", p);
    } else {
      printf("+%d ", p);

      for (int i = 0; i < 4; ++i)
        affiche_profondeur_aux(g->fils[i], p + 1);
    }
  }
}
void affiche_profondeur(image g) { affiche_profondeur_aux(g, 0); }

bool EstBlanche(image g) {
  if (g == NULL) {
    return TRUE;
  } else {
    if (g->toutnoir)
      return FALSE;

    bool res = TRUE;

    for (int i = 0; (i < 4 && res); ++i)
      res = EstBlanche(g->fils[i]);
    return res;
  }
}

bool EstNoire(image g) {
  if (g == NULL) {
    return FALSE;
  }

  if (g->toutnoir)
    return TRUE;

  bool res = TRUE;

  for (int i = 0; (i < 4 && res); ++i)
    res = EstNoire(g->fils[i]);
  return res;
}

image Copie(image g) {
  if (g == NULL) {
    return Construit_blanc();
  } else {
    if (g->toutnoir) {
      return Construit_noire();
    } else {
      return Construit_composee(Copie(g->fils[0]), Copie(g->fils[1]),
                                Copie(g->fils[2]), Copie(g->fils[3]));
    }
  }
}
static double Aire_aux(image g, int n) {
  if (g == NULL) {
    return 0;
  }
  if (g->toutnoir) {
    return 1.0 / ((double)n);
  }
  return Aire_aux(g->fils[0], n * 4) + Aire_aux(g->fils[1], n * 4) +
         Aire_aux(g->fils[2], n * 4) + Aire_aux(g->fils[3], n * 4);
}
double Aire(image g) { return Aire_aux(g, 1); }

void Rendmemoire(image g) {
  if (g != NULL) {
    for (int i = 0; i < 4; ++i)
      Rendmemoire(g->fils[i]);
    free(g);
    g = NULL;
  }
}

static int comptefils(int *i, char *s) {
  if (s == NULL || strlen(s) == (size_t)(*i)) {
    return 0;
  }
  if (s[*i] == 'N' || s[*i] == 'B') {
    (*i)++;
    return 1;
  }
  if (s[*i] == '+') {
    (*i)++;
    for (int j = 0; j < 4; ++j) {
      if (comptefils(i, s) != 1) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}
static bool ab_valide(char *s) {
  if (s == NULL || strlen(s) == 0)
    then { return FALSE; }
  else {
    if (s[0] == 'N' || s[0] == 'B')
      then { return strlen(s) == 1; }
    else {
      if (s[0] == '+')
        then {
          int i = 0;
          return comptefils(&i, s) && (i) == (int)strlen(s);
        }
      else {
        return FALSE;
      }
    }
  }
}

static char *read_from(void) {
  char c;
  char *res = (char *)malloc(1 * sizeof(char));
  int i = 0;
  printf("donner un arbre valide :: ");
  do {
    scanf("%c", &c);
    if (c == '+' || c == 'B' || c == 'N') {
      res[i] = c;
      ++i;
      res = (char *)realloc(res, (i + 1) * sizeof(char));
    }
  } while (c != '\n' && c != '\0');
  res[i] = '\0';
  if (!ab_valide(res)) {
    return NULL;
  } else {
    return res;
  }
}

image create(char *s, int *i) {

  if (s[*i] == 'B') {
    (*i)++;
    return Construit_blanc();
  }

  if (s[*i] == 'N') {

    (*i)++;
    return Construit_noire();
  }

  // if(s[*i] == '+')
  (*i)++;

  image fgh = create(s, i);
  image fdh = create(s, i);
  image fgb = create(s, i);
  image fdb = create(s, i);
  return Construit_composee(fgh, fdh, fgb, fdb);
}

image lecture(void) {
  char *ab;
  do {
    ab = read_from();
  } while (ab == NULL);

  int i = 0;
  image res = create(ab, &i);
  free(ab);
  ab = NULL;
  return res;
}

static int CompteSousImagesGrises_aux(image im, float *aire) {
  if (im == NULL)
    then {
      (*aire) = 0;
      return 0;
    }
  else {
    if (im->toutnoir)
      then {
        (*aire) = 1;
        return 0;
      }
    else {
      int res = 0;
      float somme_aire = 0.0;
      for (int i = 0; i < 4; ++i) {
        float airebis = 0.0;
        res += CompteSousImagesGrises_aux(im->fils[i], &airebis);
        somme_aire += airebis;
      }
      (*aire) = somme_aire / 4.0;
      if ((*aire) >= 1.0 / 3 && (*aire) <= 2.0 / 3) {
        res++;
      }
      return res;
    }
  }
}
int CompteSousImagesGrises(image im) {
  float aire = 0.0;
  return CompteSousImagesGrises_aux(im, &aire);
}

void Negative(image *g) {

  if ((*g) == NULL)
    then { (*g) = Construit_noire(); }

  else {
    if ((*g)->toutnoir)
      then {
        free(*g);
        (*g) = Construit_blanc();
      }
    else {
      for (int i = 0; i < 4; ++i) {
        Negative((((*g)->fils) + i)); // or &((*g)->fils[i])
      }
    }
  }
}

bool UnionNoire(image im1, image im2) {

  if (im1 == NULL)
    then { return EstNoire(im2); }
  else {
    if (im2 == NULL)
      then { return EstNoire(im1); }
    else {
      if ((im1)->toutnoir || (im2)->toutnoir)
        then { return TRUE; }
      else {
        for (int i = 0; i < 4; ++i) {
          if (!(UnionNoire(im1->fils[i], im2->fils[i])))
            then { return FALSE; }
        }
        return TRUE;
      }
    }
  }
}

void Intersection(image *im1, image im2) {

  if (im2 == NULL) {
    Rendmemoire(*im1);
    (*im1) = Construit_blanc();
  } else {
    if (!((*im1) == NULL)) {
      if ((*im1)->toutnoir)
        then {
          Rendmemoire(*im1);
          (*im1) = Copie(im2);
        }
      else {
        if (!im2->toutnoir) {
          for (int i = 0; i < 4; ++i) {
            Intersection((*im1)->fils + i, im2->fils[i]);
          }
        }
      }
    }
  }
}

/*
 *********************************************************
 *                                                       *
 *                                                       *
 *                   AFFICHAGE2KPIXEL                    *
 *                                                       *
 *********************************************************
 */

static void print_ligne(image g, int ligne, int taille) {
  if (g == NULL) {
    for (int i = 0; i < taille; ++i) {
      printf(".");
    }
  } else {
    if (g->toutnoir) {
      for (int i = 0; i < taille; ++i) {
        printf("8");
      }
    } else {
      if (taille == 1) {
        printf("_");
      } else {
        if (ligne < taille / 2) {
          print_ligne(g->fils[0], ligne, taille / 2);
          print_ligne(g->fils[1], ligne, taille / 2);
        } else {
          print_ligne(g->fils[2], ligne - taille / 2, taille / 2);
          print_ligne(g->fils[3], ligne - taille / 2, taille / 2);
        }
      }
    }
  }
}

void affichage2k(image g, int k) {
  int taille = pow(2, k);
  for (int i = 0; i < taille; ++i) {
    print_ligne(g, i, taille);
    printf("\n");
  }
}

void partage_noire(int nb_noire, int *tab, int k) {

  int tab_indices[4] = {0, 1, 2, 3};
  int taille = 4;
  int j;
  while (nb_noire != 0) {
    int i = tab_indices[j = rand() % taille];
    int x = MAX(1, rand() % MAX(1, nb_noire / 4));
    tab[i] += x;
    if ((log2l((long double)tab[i]) / 2.0) > (long double)k) {
      while ((log2l((long double)tab[i]) / 2.0) > (long double)k) {
        tab[i]--;
        x--;
      }
      tab_indices[j] = tab_indices[taille - 1];
      taille--;
    }
    nb_noire -= x;
  }
}
image creerNoeud(int k, int nb_noire) {
  if (nb_noire == 0) {
    return Construit_blanc();
  } else {
    if (((int)(log2l((long double)nb_noire) / 2.0)) == k) {
      return Construit_noire();
    } else {
      int tab[4] = {0, 0, 0, 0};
      if (nb_noire != 0) {
        partage_noire(nb_noire, tab, k - 1);
      }
      return Construit_composee(
          creerNoeud(k - 1, tab[0]), creerNoeud(k - 1, tab[1]),
          creerNoeud(k - 1, tab[2]), creerNoeud(k - 1, tab[3]));
    }
  }
}

image alea(int k, int n_pixel) {
  srand(time(NULL));
  return creerNoeud(k, n_pixel);
}