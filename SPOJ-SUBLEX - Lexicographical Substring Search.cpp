
///Time-1.70s

#include<bits/stdc++.h>

using namespace std;

string res;

/// Every node works as a state
/// There can be at most 2*n states (nodes)  total in worst case
/// Every state holds len which is the longest of the all strings of a state
/// All strings of a state have same end_of_time in original string
/// Every state holds a parameter link which holds the address of that state which holds the longest suffix of w
/// where w is the longest string of current state
/// next mapping will hold the edges between states with level char

struct state {
    int len, link;
    map<int, int> next;
};

/// This should be declared globally
/// Number of states can be at most 2*n ,where n is the length of string
/// sz for pointer of number of states in SAM i.e. Suffix Automation

const int MAXLEN = 90001;
state st[MAXLEN * 2];
int dp[2*MAXLEN];
int sz, last;

/// First state is the first one ,so len is zero
/// There is no suffix link of first state ,so link is -1
/// last is obviously zero for first state

void sa_init() {
    for (int i = 0; i <=sz; i++)
        {st[i].next.clear();}

	sz = last = 1;
	st[0].len = 0;
	st[0].link = -1;
	///++sz;
}

/// when we add new character then we will check by last pointer to which label with our new character c

void sa_extend(int c) {
    int cur = ++sz;/// increase the size of sz
    st[cur].len = st[last].len + 1;/// it's obviously increasing by one as new charcter is adding
    int p = last;
    while (p != -1 && !st[p].next.count(c)) { /// finding that state from where a state remains with edge of character same character c
        st[p].next[c] = cur;
        p = st[p].link;
    }
    if (p == -1) { /// If there is no state with such transition(edge) with character c , at last p will stop at first state
        st[cur].link = 1;/// now start from 1 node ,because it is start level now
    } else {  /// state with label c has been found
        int q = st[p].next[c]; /// the state which has been attached to p by edge c has been stated by q
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = ++sz;
            st[clone].len = st[p].len + 1;
            st[clone].next = st[q].next;
            st[clone].link = st[q].link;
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}

/// it is i.e. how many paths can be started from here + 1 (empty string)
/// So , when find kth string it will help us to find kth path as we know how many paths from here can be possible
void calcul(int v) {
    if (dp[v])
        return;
    dp[v] = 1;
    for (map<int, int>::iterator it = st[v].next.begin(); it != st[v].next.end(); it++)
    {
        calcul(it->second);
        dp[v] += dp[it->second];
    }
}

void find_kth_lexicographical(int k)
  {
        int p=1;/// our starting node now start from 1 because string start from 1 0th is #
        while(k){
            int a=0;
            while( k>dp[st[p].next[a]] && a<26 ){
                if (st[p].next[a]) k-=dp[st[p].next[a]];
                 a++;/// it is increased till there is edge from p i.e. st[p].next[a] is not empty
            }
        /// it has been checked that st[p].next[a] is not empty i.e.
        /// if it is empty so dp[0] (=0) is less than k :) ,so automatically increased by while loop
        /// so , we can take this path into account because we are finding kth path
        /// The one which path count of dp[i] is greater than k , subtract those paths from k ,new k will be found
            res+=('a'+a);k--;
            p=st[p].next[a];
        }
    }


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int n,k;
    string s;
    cin>>s;
    s="#"+s;
    sa_init();
    for(int i=1;i<s.length();i++) sa_extend(s[i]-'a');
    calcul(1);
    ///for(int i=1;i<=sz;i++) cout<<dp[i]<<endl;
    cin>>n;
    while(n--)
    {
    res="";
    cin>>k;
    find_kth_lexicographical(k);
    cout<<res<<'\n';
    }
}
