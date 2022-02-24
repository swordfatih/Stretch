# Stretch

Stretch est un langage dynamiquement typé (le type d'une variable peut changer) et les types sont inférés (il n'y a pas besoin de préciser explicitement le type d'une variable)
Le langage est conçu pour être facile d'utilisation et compréhensible par les enfants. Il y a donc quelques ressemblances avec le langage naturel ainsi que le pseudocode.

## Début du programme 

Un programme Strech démarre au début du fichier source, comme en Python ou en Javascript.

## Les commentaires

Les commentaires sont identiques à ceux présents en C++:
```
/*
    blabla
*/
```
pour des commentaires sur plusieurs lignes et
```
// blabla
```
pour un commentaire sur une seule ligne. Les commentaires sur plusieurs lignes ne sont pas *nestés*, c'est-à-dire qu'un commentaire de plusieurs lignes contenu dans un autre commentaire sur plusieurs lignes se termine dès que la première séquence `*/` est rencontrée.

## Variables

Le nom d'une variable est composé de lettres majuscules et minuscules, de chiffres et du caractère _. Cependant, le nom ne peut pas **commencer** par un chiffre. On assigne une valeur à une variable avec l'opérateur `<-`. Par exemple:
```
a <- 12
```
assigne la valeur `12` à la variable `a`.  
On peut créer plusieurs variables en une seule ligne en séparant les noms et les valeurs par une `,`. Par exemple  
```
a, b <- 12, "abcd"
```
assigne la valeur `12` à `a` et `"abcd"` à `b`. **Une assignation n'est pas une expression.**  
Les variables étant dynamiquement typées, le code suivant est valide:
```
a <- 12
a <- "abcd"
```
Les noms de variable sont sensibles à la casse. Ainsi la variable nommée `a` et la variable `A` sont deux variables différentes.

## Les types

Le langage admet plusieurs types fondamentaux:

* Les nombres entiers relatifs, comme par exemple `+17` ou `-357`. Pour un nombre positif, le signe `+` est optionnel.
* Les nombres flottants relatifs, comme par exemple `+12.35` ou `-1.2`. Comme pour les nombres entiers, le signe `+` est optionnel.
* Les booléens, qui peuvent prendre comme valeur `vrai` ou `faux`.
* Les chaînes de caractères: une suite de caractères entourée de `"`, par exemple `"bonjour"`
* Les tableaux, qui seront expliqués juste après.
* Les structures, qui seront aussi expliqués plus tard.

Les nombres peuvent contenir des `'` afin d'améliorer la lecture. Exemple:
```
a <- 3'000'000'000
```
au lieu de
```
a <- 3000000000
```

## Les tableaux

Un tableau est une suite d'éléments qui sont potentiellement de types différents. Un tableau se définit avec des crochets et les valeurs (s'il y en a) sont séparées par des virgules. Exemple:
```
a <- [1, "abcd", vrai]
```
Les éléments sont indexés à partir de l'index n°1. L'accès se fait avec `#`:
```
b <- a#2 //b == abcd
```
On peut concaténer deux tableaux avec l'opérateur `+`.
```
a <- [1, 2, 3] + [4]
// a == [1, 2, 3, 4] 
```

## Les objets

Si le programmeur souhaite créer des types plus complexes, il peut créer des types structurés. Pour cela, notre langage reprend la notion d'**objet** de *Javascript*. Par exemple, en Javascript
```js
const obj = {
    a: 12,
    b: [1, 2, 3],
    c: {
        d: "abcd"
    }
};
```
s'écrit
```
obj <- [
    a <- 12,
    b <- [1, 2, 3],
    c <- [
        d <- "abcd"
    ]
]
```
en Stretch. L'accès aux éléments se fait avec l'opérateur `de` et s'utilise de la forme `x de y`. Etant donné l'exemple précent, pour accéder à `a` on écrira `a de obj`, là où en Javascript on écrirait `obj.a`: il s'agit simplement de l'inversion de l'opérateur d'accès et cela rend la syntaxe très intuitive car proche du langage naturel.  
La syntaxe de définition d'un objet en Strech ressemble à celle de la définition d'un tableau. En effet, on peut également accéder à un élément d'un objet via son index de la même manière que dans un tableau. Toujours en prenant l'exemple précédent, le champ `a` de `obj` étant le premier défini, il a l'index 1. On peut donc y accéder en écrivant `obj#1`.

## Les opérateurs

### Opérateurs arithmétiques
Si une opération implique un nombre entier et un nombre flottant, alors le nombre entier est implicitement converti en flottant. Par exemple, `12 + 13.5` donnera `25.5`.
* l'addition, notée `+`
* La soustraction, notée `-`
* La multiplication, notée `*`
* La division, notée `/`
* Le modulo (reste de la division) noté `%`
* La négation, notée `-` qui renvoie l'opposé de l'opérande

### Opérateurs de comparaison

* l'opérateur `plus grand que` qui vérifie que l'opérande de gauche soit **strictemement** plus grand que l'opérande de droite.
* l'opérateur `plus petit que` qui vérifie que l'opérande de gauche soit **strictemement** plus petit que l'opérande de droite.
* l'opérateur `égal` (que l'on peut écrire aussi `est`) qui vérifie l'égalité des deux opérandes.
* l'opérateur `différent` (que l'on peut aussi écrire `n'est pas`) qui vérifie l'inégalité des deux opérandes.

### Opérateurs logiques

* l'opérateur `et` qui vérifie que les deux opérandes sont vrais
* l'opérateur `ou` qui vérifie qu'au moins l'un des deux opérateurs soit vrai
* l'opérateur `non` qui renvoie la valeur booléenne inverse de l'opérande

### Autres

Les parenthèses permettent de donner la priorité à une expression.

### n-arité, priorité et associativité des opérateurs

|opérateur|n-arité|priorité*|associativité|
|---------|-------|--------|-------------|
|parenthèses|unaire|0|gauche|
|de|binaire|1|droite|
|#|binaire|1|gauche|
|+ (signe)|unaire|2|droite|
|- (signe)|unaire|2|droite|
|non|unaire|2|droite|
|*|binaire|3|gauche|
|/|binaire|3|gauche|
|%|binaire|3|gauche|
|+ (addition)|binaire|4|gauche|
|- (soustraction)|binaire|4|gauche|
|plus grand que|binaire|5|gauche|
|plus petit que|binaire|5|gauche|
|égal/est|binaire|6|gauche|
|différent/n'est pas|binaire|6|gauche|
|et|binaire|7|gauche|
|ou|binaire|8|gauche|




##### *plus le nombre est petit, plus l'opérateur est prioritaire: par exemple, 1+2\*3 est évalue comme si c'était écrit 1+(2\*3) car la priorité de `*` est 3 alors que celle de `+` est 4.

## Les structures de contrôle

### Les conditions

Une condition se fait avec une structure de contrôle `si`, de la forme
```
si condition alors
  code...
fin
```
On peut aussi rajouter plusieurs conditions alternatives avec le mot-clé `sinon si`, et/ou une condition par défaut avec le mot-clé `sinon`
```
si condition1 alors
  code...
sinon si condition2 alors
  code...
sinon
  code...
fin
```
**Une condition est forcément une expression booléenne, il ne peut pas s'agir d'un nombre ou d'une chaîne de caractères par exemple.**

### Les boucles

Il y a trois types de boucle:

* La boucle `tant que`, équivalent à la boucle `while` du C
```
tant que condition alors
   code...
fin
```
**La condition est forcément une expression booléenne.**

* La boucle `répéter x fois`, qui effectue un bloc d'instruction `x` fois
```
répéter x fois
  code...
fin
```
**`x` est forcément une expression numérique entière supérieure ou égale à 0 (si x vaut 0, la boucle est ignorée)**

* La boucle `pour chaque x dans y` (boucle foreach): `y` **doit** être un tableau et `x` introduit une variable qui va référencer l'élément actuel de la boucle.

```
pour chaque i dans [1, 2, 3] alors
    code...
fin
```

Les instructions `continuer` et `sortir` permettent respectivement de passer prématurément au prochain tour de boucle de la boucle en cours et de sortir prématurément de la boucle en cours.

## Les fonctions

### Définition d'une fonction

Une fonction (séquence d'instructions qui peut renvoyer zéro, une ou plusieurs valeurs) se crée avec le mot-clé `fonction`. Exemple:
```
fonction addition <- a, b :
    bloc de code
fin
```

## Appel d'une fonction

L'appel d'une fonction se fait en indiquant son nom et la liste des paramètres entre parenthèses. Les parenthèses sont obligatoires même s'il n'y a pas de paramètre.  
Soit la fonction suivante:
```
fonction retourne_double <- a : 
    retourner a * 2
fin
```

Les conventions de nommage d'une fonction sont les mêmes que les conventions de nommage d'une variable.


Une fonction peut optionnellement retourner un ou plusieurs paramètres avec le mot-clé `retourner`.
Pour récupérer ces valeurs de retour, on peut utiliser la syntaxe de déclaration de plusieurs variables en une fois:
```
fonction renvoie_deux_valeurs :
    retourner 12, 13
fin

//appel : 
a, b <- renvoie_deux_valeurs()

//a == 12 && b == 13
```
Dans notre langage, les fonctions ne sont pas dites de *première classe*, c'est-à-dire qu'elles ne peuvent pas être stockées dans des variables, ni passées en paramètre d'une autre fonction, ni retournées depuis une autre fonction.

## Quitter

L'instruction `quitter` termine le programme lorsqu'elle est exécutée.

# Exemple de programmes en Stretch

## Somme d'une liste d'entiers

```
liste <- [2, 3, 8, 2, 3]
somme <- 0

pour chaque nombre dans liste alors
    somme <- somme + nombre
fin

afficher(nombre) //en imaginant que la fonction afficher existe
```

## Jeu du plus ou moins (en un seul essai)

```
nombre <- aleatoire()
afficher("Selon vous, quel est le nombre à deviner?")
entree <- recuperer_entree()

si entree plus grand que nombre alors
    afficher("perdu, le nombre à deviner est plus petit!")
sinon si entree égal nombre alors
    afficher("félicitations, vous avez trouvé le nombre à deviner!")
sinon
    afficher("perdu, le nombre à deviner est plus grand!")
fin
```

## Estimation de Pi avec la méthode de Monte Carlo

```
nombre_points <- 10'000
nombre_points_inclus <- 0 //le nombre de points situés dans le cercle de centre (0;0) et de rayon 1 sur le repère [0;1]

fonction distance_origine <- x, y : //renvoie la distance d'un point de coordonnées (x;y) depuis (0;0)
    retourner racine_carree(carre(x) + carre(y)) //on admet l'existence des fonctions racine_carree et carre
fin

répéter nombre_points fois
    point <- [x <- aleatoire(), y <- aleatoire()]
    //On admet que la fonction aleatoire() renvoie un nombre flottant compris entre 0 et 1
    si distance_origine(x de point, y de point) plus petit que 1.01 alors
        nombre_points_inclus <- nombre_points_inclus + 1 //le point est inclus dans le quart de cercle de rayon 1
    fin
fin

pi_sur_quatre <- nombre_points_inclus / nombre_points
pi <- 4 * pi_sur_quatre
afficher(pi)
```

# Fonctionnalités potentielles

### Opérations sur les chaînes de caractères

Concaténation de chaînes, formatage (f-strings), caractères d'échappement.

### Espaces de noms

A la manière de `namespace` en C++, permettrait de séparer les fonctionnalités. Par exemple, les fonctions de la bibliothèque standard que nous créerons pourraient être dans un espace de noms nommé `strech`.

### Traduction des mots-clés du langage

Actuellement, les mots-clés du langage sont écrits en français. Par conséquent, seuls les enfants comprenant le français peuvent se servir de Stretch. Nous souhaitons donc les traduire dans d'autres langues afin que les enfants ne parlant pas français puissent aussi programmer en Stretch.

### Bibliothèque standard du langage

Quelques fonctions usuelles afin d'améliorer l'expérience des utilisateurs et les *builtins* que nous serons obligés d'écrire avec le compilateur.

### Interfaçage avec le C

Permettrait à des contributeurs/enseignants d'ajouter des fonctionnalités au langage qui ne peuvent être écrites avec celui-ci directement.
L'interfaçage se ferait simplement via le chargement de fonctions écrites en C contenues dans des bibliothèques partagées avec `dlopen`/`dlsym` sur les systèmes POSIX et `LoadLibrary`/`GetProcAddress` sur Windows.

### Module de dessin

Ajouter des fonctions permettant au programmeur de dessiner des formes dans l'IDE fourni avec le langage (où une fenêtre indépendante) à la manière d'une *Turtle*.