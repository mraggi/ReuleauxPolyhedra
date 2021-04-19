//Las funciones necesarias para hacer cuerpos de ancho constante utilizando pedazos de esfera y arcos de revolución.
// arista(P,Q,r)
// conoaristaexterior(P1,P2,Q1,Q2)
// dibujapuntos(pts,r)
// anchoconstante(pts,r,ayo)
// acsinconvex(pts,r,ayo)

module reuleaux(pts,r) { //Hace el politopo de Reuleaux
    intersection_for(i = pts)
        translate(i) sphere(r);
}

module arista(P,Q,r) { //Crea la arista curveada por P y Q de un círculo de radio r
    d=norm(Q-P)/2;
    translate((P+Q)/2)
    rotate([0,acos((Q[2]-P[2])/(2*d)),atan2(Q[1]-P[1],Q[0]-P[0])])
    rotate_extrude()
    difference() {
        translate([-sqrt(pow(r,2)-pow(d,2)),0,0])
        circle(r);
        translate([-2*r,-r])
        square(2*r);
    }
}

module aristarecta(P,Q,r) { //Crea la arista recta
    d=norm(Q-P)/2;
    translate((P+Q)/2)
    rotate([0,acos((Q[2]-P[2])/(2*d)),atan2(Q[1]-P[1],Q[0]-P[0])])
    cylinder(norm(P-Q),r,r,center=true);
}

module dibujagrafica(pts,r,ayo) { //Dibuja la gráfica de aristas
    c=["Red","Green","Blue","Purple","Orange","Pink","Brown","Yellow","Gray","Teal"];
    for(i = [0:len(ayo)-1]) {
        color(c[i])
        union() {
            aristarecta(pts[ayo[i][0]],pts[ayo[i][1]],r);
            aristarecta(pts[ayo[i][2]],pts[ayo[i][3]],r);
        };
    }
}

module conoaristaexterior(P1,P2,Q1,Q2) { //Hace un cono sobre la arista P1P2 opuesto a Q1 y Q2
    polyhedron(
    points=[P1,P2,2*P1-Q1,2*P1-Q2,2*P2-Q1,2*P2-Q2],
    faces=[[0,2,4],[0,4,1],[1,4,5],[0,3,2],[0,5,3],[0,1,5],[2,3,4],[3,5,4]],
    convexity=4);
}

module dibujapuntos(pts,r) {
    for(i = pts)
        color("blue")translate(i)sphere(r);
}

module anchoconstante(pts,r,ayo) { //pts es la lista de puntos [P0,...,Pn-1], r es el radio de las bolas, ayo es la lista de cuartetas (eg. [0,1,5,6]) que indican la arista a curvear y su arista opuesta (eg. curvear P0P1 con opuesta P5P6).
    hull(){
        difference() {
            intersection_for(i = pts)
                translate(i)sphere(r);
            for(i = ayo)
                conoaristaexterior(pts[i[0]],pts[i[1]],pts[i[2]],pts[i[3]]);
        };
        for(i = ayo)
            arista(pts[i[0]],pts[i[1]],r);
    }
}

module acsinconvex(pts,r,ayo) { //Igual pero sin envolvente convexa.
    difference() {
        intersection_for(i = pts)
            translate(i)sphere(r);
        for(i = ayo)
            conoaristaexterior(pts[i[0]],pts[i[1]],pts[i[2]],pts[i[3]]);
    };
    for(i = ayo)
        arista(pts[i[0]],pts[i[1]],r);
}