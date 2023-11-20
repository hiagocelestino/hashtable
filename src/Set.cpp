#include "Set.hpp"

StringSet::StringSet(int tamanho) {
    tamanhoOriginal = tamanho;
    tamanhoTabela = 2 * tamanho;
    tamanhoConjunto = 0;
    tabela = new ElementoTabela[tamanhoTabela];
    for (int i = 0; i < tamanhoTabela; ++i) {
        tabela[i].vazio = true;
        tabela[i].retirada = false;
    }
}

StringSet::~StringSet() {
    delete[] tabela;
}

int StringSet::Hash(string s) {
    return s.size() % tamanhoTabela;
}

void StringSet::Rehash(int pos) {
    int i = (pos + 1) % tamanhoTabela;
    while (!tabela[i].vazio && i != pos) {
        i = (i + 1) % tamanhoTabela;
    }
    if (i != pos && tabela[i].vazio) {
        swap(tabela[pos], tabela[i]);
    }
}

void StringSet::Resize(size_t newSize) {
    ElementoTabela* novaTabela = new ElementoTabela[newSize];
    for (int i = 0; i < newSize; ++i) {
        novaTabela[i].vazio = true;
        novaTabela[i].retirada = false;
    }
    for (int i = 0; i < tamanhoTabela; ++i) {
        if (!tabela[i].vazio && !tabela[i].retirada) {
            int newHash = Hash(tabela[i].dado) % newSize;
            while (!novaTabela[newHash].vazio) {
                newHash = (newHash + 1) % newSize;
            }
            novaTabela[newHash] = tabela[i];
        }
    }
    delete[] tabela;
    tabela = novaTabela;
    tamanhoTabela = newSize;
}

void StringSet::Inserir(string s) {
    int pos = Hash(s);
    if (!Pertence(s)) {
        while (!tabela[pos].vazio && tabela[pos].dado != s && !tabela[pos].retirada) {
            pos = (pos + 1) % tamanhoTabela;
        }
        if (tabela[pos].vazio || tabela[pos].retirada) {
            tabela[pos].dado = s;
            tabela[pos].vazio = false;
            tabela[pos].retirada = false;
            ++tamanhoConjunto;
        }
        if (tamanhoConjunto * 2 > tamanhoTabela) {
            Resize(2 * tamanhoTabela);
        }
    }
}

void StringSet::Remover(string s) {
    int pos = Hash(s) % tamanhoTabela;
    while (!tabela[pos].vazio && tabela[pos].dado != s && !tabela[pos].retirada) {
        pos = (pos + 1) % tamanhoTabela;
    }
    if (!tabela[pos].vazio && tabela[pos].dado == s && !tabela[pos].retirada) {
        tabela[pos].retirada = true;
        tabela[pos].vazio = true;
        --tamanhoConjunto;
    }
}

bool StringSet::Pertence(string s) {
    int pos = Hash(s);
    while (!tabela[pos].vazio && tabela[pos].dado != s && !tabela[pos].retirada) {
        pos = (pos + 1) % tamanhoTabela;
    }
    return !tabela[pos].vazio && tabela[pos].dado == s && !tabela[pos].retirada;
}

StringSet* StringSet::Intersecao(StringSet* S) {
    StringSet* intersecao = new StringSet(tamanhoOriginal);

    for (int i = 0; i < tamanhoTabela; ++i) {
        if (!tabela[i].vazio && !tabela[i].retirada && S->Pertence(tabela[i].dado)) {
            intersecao->Inserir(tabela[i].dado);
        }
    }

    return intersecao;
}

StringSet* StringSet::Uniao(StringSet* S) {
    StringSet* uniao = new StringSet(tamanhoOriginal);
    for (int i = 0; i < tamanhoTabela; ++i) {
        if (!tabela[i].vazio && !tabela[i].retirada) {
            uniao->Inserir(tabela[i].dado);
        }
    }
    for (int i = 0; i < S->tamanhoTabela; ++i) {
        if (!S->tabela[i].vazio && !S->tabela[i].retirada) {
            uniao->Inserir(S->tabela[i].dado);
        }
    }
    return uniao;
}

StringSet* StringSet::DiferencaSimetrica(StringSet* S) {
    StringSet* diferencaSimetrica = new StringSet(tamanhoOriginal);

    for (int i = 0; i < tamanhoTabela; ++i) {
        if (!tabela[i].vazio && !tabela[i].retirada && !S->Pertence(tabela[i].dado)) {
            diferencaSimetrica->Inserir(tabela[i].dado);
        }
    }

    for (int i = 0; i < S->tamanhoTabela; ++i) {
        if (!S->tabela[i].vazio && !S->tabela[i].retirada && !Pertence(S->tabela[i].dado)) {
            diferencaSimetrica->Inserir(S->tabela[i].dado);
        }
    }

    return diferencaSimetrica;
}

void StringSet::Imprimir() {
    cout << "{ ";
    int printou = 0;
    for (int i = 0; i < tamanhoTabela; ++i) {
        if (!tabela[i].vazio && !tabela[i].retirada) {
            if (printou) {
                cout << " ,";
            }
            cout << tabela[i].dado;
            printou = 1;
        }
    }
    cout << " }" << endl;
}
