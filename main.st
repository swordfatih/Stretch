
a <- 5

fonction nombre_magique <- nombre
    entree <- 0

    afficher "Ecrivez votre nombre"
    lire dans entree

    si nombre plus grand que entree 
        afficher "📈 Plus grand"
    sinon si nombre plus petit que entree
        afficher "📉 Plus petit" 
    sinon
        sortir
    fin

    a <- nombre_magique(nombre)
fin

nombre <- aleatoire(0, 100, faux)

nombre_magique(nombre)

afficher "Félicitations, vous avez trouvé le nombre à deviner !"