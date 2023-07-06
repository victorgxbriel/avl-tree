#ifndef BSTREE_H
#define BSTREE_H
/*!
* Binary Search Tree project.
*
* @authors Victor, Daniel
* @date May 19th, 2022. 
*/

#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
#include <iostream>

std::vector<std::string> splittree(const std::string & input_str, char delimiter = ' '){
    std::vector<std::string> tokens;

    std::istringstream iss;
    iss.str(input_str);
    std::string token;
    while(std::getline(iss >> std::ws, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}

namespace edb{
/// Instancia de uma arvore de busca binaria.
template < typename T>
class Tree{
    private:
        struct Node{
            T value;
            Node *left_son;
            Node *right_son;
            int heigth_rigth;
            int heigth_left;
            int heigth;
            Node(T v=T{}, Node *n1 = nullptr, Node *n2 = nullptr, int h = 1, int hr = 0, int hl = 0) : value{v}, left_son{n1}, right_son{n2}, heigth{h}, heigth_left{hl}, heigth_rigth{hr}
            {/*empty*/}
        };
        Node *m_root;
        std::string seq_pre_ordem;
    public:
        // === Metodos regulares da classe arvore.
        /// Construtor padrão.
        Tree() : m_root{nullptr}
        {
            /*
            m_root = nullptr; // nó raiz
            */
        }
        /// Construtor iniciando com nó raiz.
        Tree(const T& value){
            m_root = new Node;
            m_root->value = value;
        }
        /// Destrutor padrão.
        ~Tree(){
            clear();
        }
        inline void aux_clear(Node * root){
            if(root == nullptr) return;

            aux_clear(root->left_son);
            aux_clear(root->right_son);

            delete root;
            root = nullptr;
        }
        /// Limpa a arvore, deixando apenas o nó raiz.
        inline void clear(){
            aux_clear(m_root);
            m_root = nullptr;
        }

        /// Função auxiliar a imprime, na maneira 1.
        inline std::string imprime1(const Node * root, int nivel, int largura){
            if(root == nullptr) return "";
            
            std::ostringstream oss;
            int spaces = nivel * 4;
            oss << std::setw(spaces) << std::setfill(' ') << "" << root->value << "" << std::setw(largura- spaces) << std::setfill('-') << "\n";
            oss << imprime1(root->left_son, nivel + 1, largura);
            oss << imprime1(root->right_son, nivel + 1, largura);

            return oss.str();

        }
        /// Função auxiliar a imprime, na maneira 2.
        inline std::string imprime2(const Node * root){
            if(root == nullptr) return "";

            std::string str = "(" + std::to_string(root->value);
            str+= imprime2(root->left_son);
            str+= imprime2(root->right_son);
            str += ")";
            return str;
        }
        /// Caso m = 1, imprime a arvore de acordo com o metodo 1, se m = 2, imprime a arvore de acordo com o metodo 2.
        inline std::string imprime_arvore(int m){
            if(m_root == nullptr) return "";
            std::string str;
            int qtd = m_root->heigth * 4;
            if(m == 1){
                str = imprime1(m_root, 0, qtd);
            } else if(m == 2){
                str = imprime2(m_root);
            } else {
                return "";
            }
            return str;
        }

        /// Função para rotação simples
        inline Node* rotacao(int balanco, Node *& node){
            Node *var = new Node;
            if(balanco < 0){ // rotacao a direita
                var = node->left_son;
                node->left_son = var->right_son;
                var->right_son = node;
            } else { // rotacao a esquerda
                var = node->right_son; 
                node->right_son = var->left_son;
                var->left_son = node;
            }
            return var;
        }
        inline Node* rotacao_dupla(int balanco, Node *& node){
            Node *neto = new Node;
            if(balanco < 0){
                Node *filho = node->left_son;
                neto = filho->right_son;
                filho->right_son = neto->left_son;
                node->left_son = neto->right_son;
                neto->left_son = filho;
                neto->right_son = node;
                // node->left_son = rotacao(1, node->left_son);
                // return rotacao(-1, node);
            } else {
                Node *filho = node->right_son;
                neto = filho->left_son;
                filho->left_son = neto->right_son;
                node->right_son = neto->left_son;
                neto->left_son = node;
                neto->right_son = filho;
                // node->right_son = rotacao(-1, node->right_son);
                // return rotacao(1, node);
            }
            return neto;
        }
        /// Função recursiva auxiliar a atualiza altura.
        inline int altura(Node *& node){
            if(node == nullptr)
                return 0;
            if(node->right_son == nullptr and node->left_son == nullptr){
                return node->heigth = 1;
            }
            int esquerda = altura(node->left_son);
            int direita = altura(node->right_son);
            node->heigth_left = esquerda;
            node->heigth_rigth = direita;
            if(esquerda > direita)
                node->heigth = esquerda + 1;
            else
                node->heigth = direita + 1;
            return node->heigth;
        }
        inline void att_altura_completa(){
            m_root->heigth_left = altura(m_root->left_son);
            m_root->heigth_rigth = altura(m_root->right_son);

            if(m_root->heigth_left > m_root->heigth_rigth)
                m_root->heigth = m_root->heigth_left + 1;
            else
                m_root->heigth = m_root->heigth_rigth + 1;
        }
        /// Função para calcular a altura da arvore, true para esquerda, false para direita
        inline void att_altura(bool dir){
            if(dir)
                m_root->heigth_left = altura(m_root->left_son);
            else 
                m_root->heigth_rigth = altura(m_root->right_son);

            if(m_root->heigth_left > m_root->heigth_rigth)
                m_root->heigth = m_root->heigth_left + 1;
            else
                m_root->heigth = m_root->heigth_rigth + 1;
        }
        inline void addcaminho(std::vector<Node *> & caminho, Node * &no){
            caminho.push_back(no);
        }
        /// Função recursiva auxiliar a remoção
        inline bool aux_remove(Node *& node, const T& value, std::vector<Node *> & caminho){
            if(node == nullptr){
                return false;
            }
            if(value > node->value){
                addcaminho(caminho, node);
                // caminho.push_back(&node);
                aux_remove(node->right_son, value, caminho);
            } else if(value < node->value){
                addcaminho(caminho, node);
                // caminho.push_back(&node);
                aux_remove(node->left_son, value, caminho);
            } else {
                if(node->left_son == nullptr and node->right_son == nullptr){
                // Caso 1 : folha
                    delete node;
                    node = nullptr;
                    return true;
                } else if(node->left_son == nullptr){
                // Caso 2 : um filho a direita
                    Node * temp = node;
                    node = node->right_son;
                    delete temp;
                    return true;
                } else if(node->right_son == nullptr){
                // Caso 2: um filho a esquerda
                    Node * temp = node;
                    node = node->left_son;
                    delete temp;
                    return true;
                } else {
                // Caso 3: 2 filhos
                // Procurar pelo menor da subarvore da direita, i.e., andar pra esquerda na sub arvore da direita
                    Node * curr = node;
                    Node * minrigh = node->right_son;
                    while(minrigh->left_son != nullptr){
                        curr = minrigh; 
                        minrigh = minrigh->left_son;
                    }
                    int vvalue = minrigh->value;
                    if(minrigh->right_son != nullptr)
                        aux_remove(curr, minrigh->value, caminho);
                    else 
                        delete minrigh;
                    m_root->value = vvalue;
                    return true;
                }
            }
        }
        inline void att_caminho(std::vector<Node *> & caminho){
            for(int i = caminho.size() - 1; i > -1; i--){
                Node * rot = new Node;
                Node * pai = caminho[i];
                int balanco = pai->heigth_rigth - pai->heigth_left;
                if(balanco == 2){
                    Node * filho = pai->right_son;
                    int balanco2 = filho->heigth_rigth - filho->heigth_left;
                    if(balanco2 > 0)
                        rot = rotacao(balanco, pai);
                    else
                        rot = rotacao_dupla(balanco, pai);
                    if(i == 0)
                        m_root = rot;
                    else {
                        Node * avo = caminho[i-1];
                        if(avo->right_son->value == pai->value)
                            avo->right_son = rot;
                        else
                            avo->left_son = rot;
                    }
                } else if(balanco == -2){
                    Node* filho = pai->left_son;
                    int balanco2 = filho->heigth_rigth - filho->heigth_left;
                    if(balanco2 > 0)
                        rot = rotacao_dupla(balanco, pai);
                    else 
                        rot = rotacao(balanco, pai);
                    if(i == 0) // caso for a raiz da arvore
                        m_root = rot;
                    else{ // cc
                        Node * avo = caminho[i-1];
                        if(avo->right_son->value == pai->value)
                            avo->right_son = rot;
                        else
                            avo->left_son = rot;
                    }
                }
                att_altura_completa();
            }
        }
        /// Remove o nó da arvore cujo valor passado, caso não esteja na arvore retorna -1.
        inline int remove(const T & value){
            bool flag;
            std::vector<Node *> caminho;
            if(m_root != nullptr){
                Node * node = m_root;
                flag = aux_remove(m_root, value, caminho);
            }
            if(flag and m_root != nullptr){
                if(value == m_root->value)
                    att_altura(false);
                else if( value > m_root->value)
                    att_altura(false);
                else 
                    att_altura(true);

                att_caminho(caminho);
                att_altura_completa();
            }
            if(flag)
                return value;
            return -1;
        }
        /// Retorna o nó caso esteja na arvora, e -1, caso contrario.
        inline bool busca(const T & value){
            Node *var = new Node;
            // Verifica se a arvore é vazia
            if(m_root != nullptr){
                var = m_root;
                while(true){
                    // Verifica se o valor do nó é o procurado
                    if(var->value == value){
                        return true;
                    } else if(var->value > value and var->left_son != nullptr){
                        var = var->left_son;
                    } else if(var->value < value and var->right_son != nullptr){
                        var = var->right_son;
                    } else {
                        return false;
                    }
                }
            }
        }
        /// Função auxiliar a insert. Retorna true, caso a inserção for em sucessida, e false caso contrario(Valor já está na arvore).
        inline bool var_insert(Node * root, Node * newnode, int & h, bool & att, std::vector<Node *> &caminho){
            caminho.push_back(root);
            if(newnode->value == root->value){
                return false;
            }
            // direita
            if(newnode->value > root->value){
                if(!root->right_son){
                    h = 1;
                    root->right_son = newnode;
                    root->heigth_rigth = newnode->heigth;
                    if(root->heigth_left < root->heigth_rigth){
                        root->heigth = root->heigth_rigth + h;
                        att = true;
                    }
                    return true;
                } else {
                    if(var_insert(root->right_son, newnode, h, att, caminho)){
                        if(att)
                            root->heigth_rigth += h;
                        if(root->heigth_left < root->heigth_rigth)
                            root->heigth = root->heigth_rigth+ h;
                    }
                }
                // esquerda
            } else if(newnode->value < root->value){
                if(!root->left_son){
                    h = 1;
                    root->left_son = newnode;
                    root->heigth_left = newnode->heigth;
                    if(root->heigth_left > root->heigth_rigth){
                        root->heigth = root->heigth_left + h;
                        att = true;
                    }
                    return true;
                } else {
                    if(var_insert(root->left_son, newnode, h, att, caminho)){
                        if(att)
                            root->heigth_left += h;
                        if(root->heigth_left > root->heigth_rigth)
                            root->heigth = root->heigth_left + h;
                    }
                }
            }
        }

        inline void att_caminho_add(std::vector<Node *> &caminho){
            for(int i = caminho.size() - 1; i > -1; i--){
                Node * rot = new Node;
                Node * pai = caminho[i];
                int balanco = pai->heigth_rigth - pai->heigth_left;
                if(balanco == 2){
                    Node * filho = pai->right_son;
                    int balanco2 = filho->heigth_rigth - filho->heigth_left;
                    if(balanco2 > 0)
                        rot = rotacao(balanco, pai);
                    else
                        rot = rotacao_dupla(balanco, pai);
                    if(i == 0)
                        m_root = rot;
                    else {
                        Node * avo = caminho[i-1];
                        if(avo->right_son->value == pai->value)
                            avo->right_son = rot;
                        else
                            avo->left_son = rot;
                    }
                    break;
                } else if(balanco == -2){
                    Node* filho = pai->left_son;
                    int balanco2 = filho->heigth_rigth - filho->heigth_left;
                    if(balanco2 > 0)
                        rot = rotacao_dupla(balanco, pai);
                    else 
                        rot = rotacao(balanco, pai);
                    if(i == 0) // caso for a raiz da arvore
                        m_root = rot;
                    else{ // cc
                        Node * avo = caminho[i-1];
                        if(avo->right_son->value == pai->value)
                            avo->right_son = rot;
                        else
                            avo->left_son = rot;
                    }
                    break;
                }
            }
        }
        /// Insere um nó na arvore com o valor passado, caso já esteja na arvore retorna -1.
        inline int insert(const T & value){
            bool flag;
            std::vector<Node*> caminho;
            Node *newnode = new Node;
            newnode->value = value;
            if(m_root == nullptr){
                m_root = newnode;
                flag = true;
            } else {
                int v = 0;
                int& h = v;
                bool x = false;
                bool& att = x;
                flag = var_insert(m_root, newnode, h, att,caminho);
                /*
                newnode = aux_insert(m_root, value);
                if(newnode != m_root)
                    return -1;
                */
            }
            if(flag){
                att_caminho_add(caminho);
                att_altura_completa();
                return value;
            }else
                return -1;
        }

};
}
#endif