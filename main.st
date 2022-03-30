nombre_points <- 10'000
nombre_points_inclus <- 0  

fonction distance_origine <- x, y  
    sortir avec racine(puissance(x, 2) + puissance(y, 2), 2)  
fin

repeter nombre_points fois dans i
    point <- [aleatoire(0, 1, vrai), aleatoire(0, 1, vrai)]
    distance <- distance_origine(point#1, point#2)

    si distance plus petit que 1 ou distance egal 1 alors
        nombre_points_inclus <- nombre_points_inclus + 1  
    fin
fin

pi_sur_quatre <- nombre_points_inclus / nombre_points
pi <- 4 * pi_sur_quatre
afficher pi