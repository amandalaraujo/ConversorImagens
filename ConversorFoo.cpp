// Exercicio - Realizar o armazenamento, compactacao e reproducao de imagens.
/*
    Requisitos do Exercicio:
    - Considere que o arquivo de armazenamento de uma imagem com extensao .foo segue o seguinte padrao:
    ● Uma largura, formatada como caracteres ASCII em decimal.
    ● Espaco em branco.
    ● Uma altura, novamente em decimal ASCII.
    ● Um caractere de final de linha.
    ● Uma sequencia de bytes representando, respectivamente, cada pixel da imagem. 
    Obs: cada byte pode assumir valores entre 0 e 255.

    - Os arquivos com extensao .foo2 devem possuir a seguinte estrutura:
    ● Uma largura, formatada como caracteres ASCII em decimal.
    ● Espaco em branco.
    ● Uma altura, novamente em decimal ASCII.
    ● Um caractere de final de linha.
    ● Uma sequencia de caracteres ASCII com diferentes densidades representando, respectivamente, cada pixel da imagem conforme
        especificação a seguir: $@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,"^`.
    Obs: Caracteres ordenados por densidade descendente.
    Obs: Parametros para execucao: ./a.out img1.foo img2.foo2 -> Onde img1.foo eh a imagem original, img2.foo2 eh a imagem convertida
*/

#include <iostream>
#include <fstream> // Leitura e escrita de arquivos

using namespace std;

//=== STRUCT PARA ARMAZENAR AS DIMENSOES DAS IMAGENS ===//
struct imgBinario
{
    int largura; // Largura da imagem
    int altura; // Altura da imagem
};

typedef struct imgBinario imgB;

// Converter os valores dos pixels da imagem em texto ASCII -> "$" eh mais escuro, ";" eh mais claro"
const string strCaracteres = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?- _ + ~<>i!lI; :,^`.";

//=== FUNCAO PARA LER UM ARQUIVO .FOO ===//
void readFile(const string &filename, imgB &img, unsigned char*& pixels) { 
    ifstream file(filename, ios::binary); // Abre o arquivo em modo binario.

    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo .foo" << endl; // Se nao abrir, mostra erro e encerra o programa
        exit(1);
    }

    file >> img.largura >> img.altura;
    file.ignore(); // Ignora o caractere de nova linha

    // Alocando memoria para armazenar os pixels
    pixels = new unsigned char[img.largura * img.altura];
    file.read(reinterpret_cast<char*>(pixels), img.largura * img.altura); // Le os pixels
}

//=== FUNCAO PARA CONVERTER PIXELS EM CARACTERES ===//
char pixelChar(unsigned char pixelValor) {
    int indice = pixelValor * (strCaracteres.size() - 1) / 255;
    return strCaracteres[indice];
}

//=== FUNCAO PARA ESCREVER UM ARQUIVO .FOO2 ===//
void writeFile(const string &filename, const imgB& img, const unsigned char* pixels) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo .foo2" << endl;
        exit(1);
    }

    file << img.largura << " " << img.altura << "\n"; // Escreve a largura e altura da imagem

    for (int i = 0; i < img.altura; ++i) {
        for (int j = 0; j < img.largura; ++j) {
            file << pixelChar(pixels[i * img.largura + j]);
        }
        file << "\n"; // Adiciona uma nova linha apos cada linha de caracteres
    }

    file.close();
}

// Adicionado passagem por parametro na main -> chamada via interface de linha de comando
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <input_file.foo> <output_file.foo2>" << endl;
        return 1;
    }

    string inFilename = argv[1];
    string outFilename = argv[2];

    imgB img;
    unsigned char* pixels = nullptr;

    try {
        readFile(inFilename, img, pixels);
        // Exibir quantidade de linhas e colunas da imagem original
        cout << "Dimensoes da imagem: " << img.largura << "x" << img.altura << "\n";
        writeFile(outFilename, img, pixels);
        delete[] pixels; 
    }
    catch (const exception& e) {
        cerr << e.what() << "\n";
        if (pixels) {
            delete[] pixels; // Liberacao de memoria em caso de excecao
        }
        return -1;
    }

    return 0;
}