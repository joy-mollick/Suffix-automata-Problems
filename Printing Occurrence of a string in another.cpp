
///Where every case , s is main text and t string should be found by its occurrence 
/// Implementation

#include<bits/stdc++.h>

using namespace std;

string t;

/// Every node works as a state
/// There can be at most 2*n states (nodes)  total in worst case
/// Every state holds len which is the longest of the all strings of a state
/// All strings of a state have same end_of_time in original string
/// Every state holds a parameter link which holds the address of that state which holds the longest suffix of w
/// where w is the longest string of current state
/// next mapping will hold the edges between states with level char

struct state {
    bool is_clone;
    int len, link,pos;
    map<int, int> next;
    vector<int>inv_link; ///normal link states the state of longest suffix of current state's longest string but inv_link container store from which states this state can be obtained by suffix link
};

/// This should be declared globally
/// Number of states can be at most 2*n ,where n is the length of string
/// sz for pointer of number of states in SAM i.e. Suffix Automation

const int MAXLEN = 1000001;
state st[MAXLEN * 2];
int sz, last;

/// First state is the first one ,so len is zero
/// There is no suffix link of first state ,so link is -1
/// last is obviously zero for first state

void sa_init() {
    for (int i = 0; i <=sz; i++)
        {st[i].next.clear();st[i].inv_link.clear();}

	sz = last = 0;
	st[0].len = 0;
	st[0].link = -1;
	++sz;
}

/// when we add new character then we will check by last pointer to which label with our new character c

void sa_extend(char c) {
    int cur = sz++;/// increase the size of sz
    st[cur].len = st[last].len + 1;/// it's obviously increasing by one as new charcter is adding
    st[cur].pos=st[cur].len-1;/// it holds for the position where the longest suffix of state cur ends according to 0 based index
    st[cur].is_clone=false;
    int p = last;
    while (p != -1 && !st[p].next.count(c)) { /// finding that state from where a state remains with edge of character same character c
        st[p].next[c] = cur;
        p = st[p].link;
    }
    if (p == -1) { /// If there is no state with such transition(edge) with character c , at last p will stop at first state
        st[cur].link = 0;
    } else {  /// state with label c has been found
        int q = st[p].next[c]; /// the state which has been attached to p by edge c has been stated by q
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = sz++;
            st[clone].len = st[p].len + 1;
            st[clone].next = st[q].next;
            st[clone].link = st[q].link;
            st[clone].pos=st[clone].len-1;/// holding end position of longest suffix of this state
            st[clone].is_clone=true;/// marked as clone node
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}

/// inv_link container store the inv_link of states

void cal_cul()
{
    for(int i=1;i<sz;i++)
    {
        st[st[i].link].inv_link.push_back(i);
    }
}

/// now through the dfs to find the all occurrences of the state containing the string say i.e. t . because it is obvious that the end_position of suffixes of the given string is also occurrence of current string

void dfs(int v,vector<int>&V)
{
    if(!st[v].is_clone)/// if it is not clone ,then print because it is duplicate of exact state
       {V.push_back(st[v].pos-t.length()+1);}

    int sz=st[v].inv_link.size();
    for(int i=0;i<sz;i++)
    {
        int u=st[v].inv_link[i];
        dfs(u,V);
    }
}

void find_state(vector<int>&V)
{
    int pointer=0;
    int match=0;
    int i=0;

    while(st[pointer].next[t[i]])
    {
        match++;
        pointer=st[pointer].next[t[i]];
        i++;
    }

    if(match==t.length())
    {
        dfs(pointer,V);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int n;
    string s;
    cin>>n;
    while(n--)
    {
    cin>>s>>t;
    vector<int>V;
    sa_init();
    for(int i=0;i<s.length();i++) sa_extend(s[i]);
    cal_cul();
    find_state(V);
    sort(V.begin(),V.end());
    int siz=V.size();
    if(siz>0) {cout<<siz<<endl;
    for(int i=0;i<siz;i++) cout<<V[i]+1<<" ";}
    else {cout<<"Not Found";}
    cout<<endl;
    }
}
