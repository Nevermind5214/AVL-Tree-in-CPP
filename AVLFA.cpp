#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int lforgatva = 0;
int rforgatva = 0;

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

struct node {
    struct node* bal;
    struct node* jobb;
    int adat;
};

class AVL
{
public:
    struct node* gyoker;
    AVL() {
        this->gyoker = NULL;
    }

    struct node* avl_beszur(struct node* gymut, int ertek) {

        if (gymut == NULL) {
            gymut = new node;
            gymut->bal = NULL;
            gymut->jobb = NULL;
            gymut->adat = ertek;
            return gymut;
        }

        else if (gymut->adat > ertek)
            gymut->bal = avl_beszur(gymut->bal, ertek);

        else if (gymut->adat < ertek)
            gymut->jobb = avl_beszur(gymut->jobb, ertek);

        else  std::cout << std::endl << "Mar van ilyen erteku elem!" << std::endl;

        int e = egyen_szam(gymut);

        if (e > 1) {
            if (ertek > gymut->bal->adat)
                gymut->bal = l_forgat(gymut->bal);
            gymut = r_forgat(gymut);
        }

        else if (e < -1) {
            if (ertek < gymut->jobb->adat)
                gymut->jobb = r_forgat(gymut->jobb);
            gymut = l_forgat(gymut);
        }

        return gymut;
    }

    int egyen_szam(struct node* gymut) {
        if (gymut == NULL) return 0;
        return magassag(gymut->bal) - magassag(gymut->jobb);
    }

    int magassag(struct node* gymut) {
        int bal = 0;
        int jobb = 0;
        if (!gymut) return 0;
        if (gymut->bal) bal = magassag(gymut->bal);
        if (gymut->jobb) jobb = magassag(gymut->jobb);
        return std::max(bal, jobb) + 1;
    }

    struct node* r_forgat(struct node* z) {
        rforgatva++;
        struct node* x = z->bal;
        z->bal = x->jobb;
        x->jobb = z;
        return x;
    }

    struct node* l_forgat(struct node* x) {
        lforgatva++;
        struct node* z = x->jobb;
        x->jobb = z->bal;
        z->bal = x;
        return z;
    }

    void kiir(struct node* v, int tabs) {
        if (!v) return;
        kiir(v->jobb, tabs + 5);
        for (int k = 0; k < tabs - 5; k++)  std::cout << "  ";
        if (tabs > 0) {
            std::cout << "  |--------";
        }
        std::cout << v->adat << std::endl;
        kiir(v->bal, tabs + 5);
    }


    struct node* avl_torol(struct node* gymut, int ertek) {
        if (gymut == NULL) std::cout << std::endl << "Nincs ilyen erteku elem!" << std::endl;
        else if (gymut->adat > ertek) gymut->bal = avl_torol(gymut->bal, ertek);
        else if (gymut->adat < ertek) gymut->jobb = avl_torol(gymut->jobb, ertek);
        else if (gymut->bal == NULL) gymut = gymut->jobb;
        else if (gymut->jobb == NULL) gymut = gymut->bal;
        else {
            struct node* legbal = gymut->jobb;
            while (legbal->bal != NULL) legbal = legbal->bal;
            gymut->adat = legbal->adat;
            gymut->jobb = avl_torol(gymut->jobb, legbal->adat);
        }
        int e = egyen_szam(gymut);
        if (e > 1) {
            if (egyen_szam(gymut->bal) < 0) gymut->bal = l_forgat(gymut->bal);
            gymut = r_forgat(gymut);
        }
        else if (e < -1) {
            if (egyen_szam(gymut->jobb) > 0) gymut->jobb = r_forgat(gymut->jobb);
            gymut = l_forgat(gymut);
        }
        return gymut;


       
    }

    void preorder() {
        preOrderHelper(this->gyoker);
    }

    void inorder() {
        inOrderHelper(this->gyoker);
    }

    void postorder() {
        postOrderHelper(this->gyoker);
    }

    void preOrderHelper(node* mynode) {
        if (mynode) {
            std::cout << mynode->adat << " ";
            preOrderHelper(mynode->bal);
            preOrderHelper(mynode->jobb);
        }
    }

    void inOrderHelper(node* mynode) {
        if (mynode) {
            inOrderHelper(mynode->bal);
            std::cout << mynode->adat << " ";
            inOrderHelper(mynode->jobb);
        }
    }

    void postOrderHelper(node* mynode) {
        if (mynode) {
            postOrderHelper(mynode->bal);
            postOrderHelper(mynode->jobb);
            std::cout << mynode->adat << " ";
        }
    }

    ~AVL() {

    }

};

int main() {
    std::cout << "AVL fa epit, torol, bejar" << std::endl << "Torleshez irj: \"t\"" << std::endl;
    AVL fa;
    std::string besor;

    while (true) {
        std::cout << "Uj ertek: ";
        std::cin >> besor;

        if (is_number(besor)) fa.gyoker = fa.avl_beszur(fa.gyoker, stoi(besor));
        else if (besor[0] == 't') {
            std::cout << "Torlendo elem: ";
            std::cin >> besor;
            if (is_number(besor)) fa.gyoker = fa.avl_torol(fa.gyoker, stoi(besor));
            else std::cout << std::endl << "Ervenytelen bemenet!" << std::endl;
        }
        else std::cout << std::endl << "Ervenytelen bemenet!" << std::endl;


        std::cout << std::endl << "-------* l_forgat: " << lforgatva << " r_forgat: " << rforgatva << " *-------" << std::endl << std::endl;
        fa.kiir(fa.gyoker, 0);
        std::cout << std::endl;
        std::cout << "Preorder bejaras: ";
        fa.preorder();
        std::cout << std::endl << "Inorder bejaras: ";
        fa.inorder();
        std::cout << std::endl << "Postorder bejaras: ";
        fa.postorder();
        std::cout << std::endl;
    }
}