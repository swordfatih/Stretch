a <- 5 * (-1)
b <- 5 * -a
c <- b egal a
d <- non c

f <- 0
tant que non c
    f <- f + 1

    si f egal 100
        c <- vrai
    fin
fin

repeter 5 fois dans i
    e <- "ca repete"
fin

si faux
    e <- "ca marche"
fin

fonction factorielle <- i:
    si i plus petit que 2
        retourner 1
    fin
    
    retourner i * factorielle(i - 1) 
fin

c <- factorielle(10) || 3628800 ||