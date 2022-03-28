nombre, entree <- aleatoire(0, 100, faux), 0

tant que vrai | entree different nombre
    afficher "ecrivez votre nombre\n"
    lire dans entree

    si nombre plus grand que entree 
        afficher "plus grand\n"
    sinon si nombre plus petit que entree
        afficher "plus petit\n"
    sinon
        arreter
    fin
fin

afficher "Félicitations, vous avez trouvé le nombre à deviner !\n"