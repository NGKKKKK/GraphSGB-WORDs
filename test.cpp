#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

int MAX_SIZE = 5;

//Tạo kiểu dữ liệu của đỉnh (lưu giá trị, các đỉnh kề, đã thăm hay chưa?)
struct Vertex{
    string data;
    vector<Vertex*> friends;
    bool visited = false;

    Vertex(const string& input){
        data = input;
    }
    Vertex(){
        data = "";
    }
    bool isFriend(const Vertex* v){
        int c = 0;
        for (int i = 0; i < MAX_SIZE; ++i){
            if (data[i] != v->data[i]) ++c;
        }
        return (c == 1);
    }
    void addFriend(Vertex* v){
        friends.push_back(v);
    }
};

//Dùng để đếm số thành phần liên thông (duyệt DFS thôi)
void DFS(Vertex* v){
    v->visited = true;
    for (Vertex* v1 : v->friends){
        if (!v1->visited) DFS(v1);
    }
}

//Đếm số thành phần liên thông
int countComponents(vector<Vertex*>& Graph){
    int cnt = 0;
    for (Vertex* v : Graph){
        if (!v->visited){
            DFS(v);
            ++cnt;
        }
    }
    return cnt;
}

//Gán cho các đỉnh là chưa thăm (đưa về đồ thị về ban đầu lúc mới tạo)
void resetGraph(vector<Vertex*>& Graph){
    for (Vertex* v : Graph) v->visited = false;
}

//Tìm kiếm đỉnh có giá trị với đầu vào
Vertex* findVertexWithWord(const string& word, const vector<Vertex*>& Graph){
    for (Vertex* v : Graph){
        if (v->data == word) return v;
    }
    return nullptr;
}

//Tìm đường đi ngắn nhất giữa 2 đỉnh (u -> v) (không trọng số thì BFS thôi là được)
vector<Vertex*> findShortestPath(Vertex* u, Vertex* v){
    queue<Vertex*> que;
    unordered_map<Vertex*, Vertex*> trace;
    u->visited = true;
    que.push(u);
    while (!que.empty()){
        Vertex* cur = que.front();
        que.pop();
        if (cur == v){
            vector<Vertex*> res;
            Vertex* tmp = v;
            while (tmp != nullptr){
                res.push_back(tmp);
                tmp = trace[tmp];
            }
            return res;
        }
        for (Vertex* x : cur->friends){
            if (!x->visited){
                x->visited = true;
                trace[x] = cur;
                que.push(x);
            }
        }
    }
    return {};
}

//Xử lý chính
int main(){
    SetConsoleOutputCP(65001);
    vector<Vertex*> Graph;
    //Đọc dữ liệu các từ
    ifstream file("sgb-words.txt");
    if (!file.is_open()){
        cerr << "ERROR!" << endl;
        return 0;
    }
    string word;
    while (file >> word){
        //file.ignore();
        //cout << "#" << word << endl;
        Vertex* newVertex = new Vertex(word);
        Graph.push_back(newVertex);
    }
    file.close();
    //Xử lí các đỉnh kề với nhau
    int sz = Graph.size();
    //cout << Graph[sz-1]->data << endl;
    for (int i = 0; i < sz; ++i){
        for (int j = i+1; j < sz; ++j){
            if (Graph[i]->isFriend(Graph[j])){
                Graph[i]->addFriend(Graph[j]);
                Graph[j]->addFriend(Graph[i]);
            }
        }
    }
    //Phần a
    int ans1 = countComponents(Graph);
    cout << "a) Số thành phần liên thông là: " << ans1 << endl;
    resetGraph(Graph);
    //Phần b
    cout << "b) " << endl;
    string w1,w2;
    cout << "Từ 1 : "; cin >> w1;
    cout << "Từ 2 : "; cin >> w2;
    Vertex* u = findVertexWithWord(w1, Graph);
    Vertex* v = findVertexWithWord(w2, Graph);
    if (u == nullptr || v == nullptr){
        cout << "Lỗi đầu vào : Không tìm thấy từ hợp lệ !" << endl;
        return 0;
    }
    vector<Vertex*> ans2 = findShortestPath(u,v);
    int sz_ = ans2.size();
    if (sz_ <= 1){
        cout << "Không có đường đi ngắn nhất giữa 2 đỉnh đã cho" << endl;
        return 0;
    }
    cout << "Đường đi ngắn nhất giữa 2 đỉnh đã cho có độ dài là: " << sz_-1 << endl << "Đường đi: ";
    for (int i = sz_-1; i > 0; --i) cout << ans2[i]->data << " -> ";
    cout << ans2[0]->data;
    cout << endl;
    for (Vertex* kl : Graph) delete kl;
    return 0;
}
