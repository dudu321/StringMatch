#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> buffer;
int bf_s = 0, bf_l = 0;
string min_t, cmp;
int min_offset;

inline int fab(int x) {
    int s = buffer.size();
    return (x % s + s) % s;
}
bool increase() {
    int pre = 0;
    for(int i = bf_l - 1, p; i >= 0 && pre == 0; i--) p = fab(bf_s + i), pre = buffer[p] = (buffer[p] + 1) % 10;
    return pre == 0;
}
void decrease() {
    int pre = 9;
    for(int i = bf_l - 1, p; i >= 0 && pre == 9; i--) p = fab(bf_s + i), pre = buffer[p] = (buffer[p] + 9) % 10;
    while(buffer[fab(bf_s)] == 0 && bf_l > 1) ++bf_s, --bf_l;
}
bool operator < (const string &a, const string &b) {
    if(a.size() != b.size()) return a.size() < b.size();
    for(int i = 0; i < a.size(); i++) if(a[i] != b[i]) return a[i] < b[i];
    return false;
}
void f(string str, stringstream &sout) {
    for(auto &c : str) c -= '0';
    min_t.clear();
    for(int l = 1; l <= str.size(); l++) for(int t = 1; t <= l; t++) {
        bf_s = bf_l = 0;
        int i = -1, cur_l = l, nxt_s = t, len = t;
        if(t == l && str[0] == 0) continue;
        while(++i < str.size()) {
            if(i == nxt_s) {
                if(str[i] == 0) break;
                bf_s = fab(bf_s + bf_l - len), bf_l = len;
                if(increase()) {
                    if(len == cur_l) buffer[fab(--bf_s)] = 1, ++bf_l, ++cur_l;
                    else {
                        if(i + cur_l < str.size() && str[i + cur_l] == 0) ++cur_l;
                        else if(i + cur_l >= str.size()) if(i + cur_l - bf_l < str.size() && str[i + cur_l - bf_l] != 0) ++cur_l;
                    }
                }
                nxt_s = i + cur_l;
                len = min(nxt_s, (int)str.size()) - i;
            }
            if(bf_l - nxt_s + i >= 0) if(buffer[fab(bf_s + bf_l - nxt_s + i)] != str[i]) break;
            buffer[fab(bf_s + bf_l - nxt_s + i)] = str[i];
        }
        if(i == str.size()) {
            bf_s = fab(bf_s + bf_l - cur_l), bf_l = cur_l;
            cmp.resize(bf_l);
            for(int j = 0; j < bf_l; j++) cmp[j] = buffer[fab(bf_s + j)];
            if(min_t.empty() || cmp < min_t) min_t = cmp, min_offset = nxt_s - i + 1;
            else if(min_t == cmp) min_offset = max(min_offset, nxt_s - i + 1);
        }
    }

    int i = str.size() - 1, nxt_s = i + min_offset;
    bf_s = 0, bf_l = min_t.size();
    for(int j = 0; j < min_t.size(); j++) buffer[fab(bf_s + j)] = min_t[j];
    for(int cnt = 0; cnt < 10; cnt++) {
        if(++i == nxt_s) {
            if(increase()) buffer[fab(--bf_s)] = 1, ++bf_l;
            nxt_s += bf_l;
        }
        sout << buffer[fab(bf_s + bf_l - nxt_s + i)];
    }
    sout << endl;
    for(int cnt = 0; cnt < 10;) {
        if(i-- == nxt_s - bf_l) decrease(), nxt_s = i + 1;
        if(i < 0) sout << buffer[fab(bf_s + bf_l - nxt_s + i)], cnt++;
    }
}

