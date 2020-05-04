
/*
smallest cycle shift is the smallest lexicographically shortest string
which can be obtained by shifting some characters from beginning to the end.
aba -> baa -> aab . These are the strings obtained by cycle-shifts.
among them aab is the lexicographically smallest .To achieve this,we have to shift first two characters .
So minimum shift is two 
*/


#include<bits/stdc++.h>

using namespace std;
/// Every node works as a state
/// There can be at most 2*n states (nodes)  total in worst case
/// Every state holds len which is the longest of the all strings of a state
/// All strings of a state have same end_of_time in original string
/// Every state holds a parameter link which holds the address of that state which holds the longest suffix of w
/// where w is the longest string of current state
/// next mapping will hold the edges between states with level char

struct state {
    int len, link,ending_pos;
    map<int, int> next;
};

/// This should be declared globally
/// Number of states can be at most 2*n ,where n is the length of string
/// sz for pointer of number of states in SAM i.e. Suffix Automation

const int MAXLEN = 10001;
state st[MAXLEN * 2];
int sz, last;
/// First state is the first one ,so len is zero
/// There is no suffix link of first state ,so link is -1
/// last is obviously zero for first state

void sa_init() {
    for (int i = 0; i <=sz; i++)
       st[i].next.clear();

	sz = last = 0;
	st[0].len = 0;
	st[0].link = -1;
	++sz;
}

/// when we add new character then we will check by last pointer to which label with our new character c

void sa_extend(int c) {
    int cur = sz++;/// increase the size of sz
    st[cur].len = st[last].len + 1;/// it's obviously increasing by one as new charcter is adding
    st[cur].ending_pos=st[cur].len-1;/// we will consider 1 based indexing here ,so that it can be actually length of prefix also
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
            st[clone].ending_pos=st[q].ending_pos;
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}

/// smallest cycle shift finding

///
int pos_of_end_of_smallest_one;

void dfs(int state,int len)
{
    if(len==0){pos_of_end_of_smallest_one=st[state].ending_pos; return;}/// finding smallest string with length (n/2) of original length of string
    for(int i=0;i<26;i++)
    {
        if(st[state].next[i])
        {
            dfs(st[state].next[i],len-1);
            return;
        }
    }
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int n;
    cin>>n;
    while(n--)

    {

    string s;
    cin>>s;
    sa_init();
    s=s+s;
    for(int i=0;i<s.length();i++)
    {
            sa_extend(s[i]-'a');
    }
    int siz=s.size();
    int original_length=siz>>1;
    dfs(0,original_length);
    int shift=pos_of_end_of_smallest_one-original_length+1;/// number of characters from the beginning should be added to the end of the original string
    cout<<shift<<'\n';

    }

    }
