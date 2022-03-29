
nombre, entree <- aleatoire(0, 100, faux), 0

tant que vrai 
    afficher "Ecrivez votre nombre"
    lire dans entree

    si nombre plus grand que entree 
        afficher "📈 Plus grand"
    sinon si nombre plus petit que entree
        afficher "📉 Plus petit"
    sinon
        arreter
    fin
fin

afficher "Félicitations, vous avez trouvé le nombre à deviner !"