/*************************************************/
/*                                               */
/*                sucre syntaxique               */
/*                                               */
/*************************************************/

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define AND &&
#define OR ||
#define ISNOT !=
#define NOT !
#define then

typedef enum { FALSE, TRUE } bool;
/*************************************************/
/*                                               */
/*          definition type image                */
/*                                               */
/*************************************************/
typedef struct bloc_image {
  bool toutnoir;
  struct bloc_image *fils[4];
} bloc_image;

typedef bloc_image *image;

/*************************************************/
/*                                               */
/*                predeclarations                */
/*                                               */
/*************************************************/

image Construit_blanc();

image Construit_noire();

image Construit_composee(image ihg, image ihd, image ibg, image ibd);


void affiche_normal(image g);

void affiche_profondeur(image g);

bool EstBlanche(image g);

bool EstNoire(image g);

image Copie(image g);

double Aire(image g);

void Rendmemoire(image g);

image lecture(void);

int CompteSousImagesGrises(image g);

void Negative(image *g);

bool UnionNoire(image im1, image im2);

void Intersection(image * im1, image im2);

void affichage2k(image g, int k);

image creerNoeud(int n,int n_pix);

image alea(int k,int n_pixel);
