/// Very Nice problem 
/// Time-0.630s



/*

It is known to all that every state of SAM holds the length of maximum length in it's len parameter of that particular state .
Now  if we can count the number of times longest suffix of a state with length len has been occurred .
We can count the number of occurrence of each state's longest strings .
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
    int len, link;
    map<int, int> next;
};

/// This should be declared globally
/// Number of states can be at most 2*n ,where n is the length of string
/// sz for pointer of number of states in SAM i.e. Suffix Automation

const int MAXLEN = 250001;
int ans[MAXLEN];
state st[MAXLEN * 2];
int cnt[2*MAXLEN]; ///It holds for the size of each endpos set of occurrence of each state ,because each state corresponds to an endpos set of all occurrence of those string in that set ,so we need that size
int sz, last;
set< pair<int,int> > cal;/// It is for aid of calculation of each state endpos set class size,it will be sorted according to the size of long suffix of a state

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
    cnt[cur]=1;
    cal.insert(make_pair(st[cur].len,cur));
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
            cnt[clone]=0;
            cal.insert(make_pair(st[clone].len,clone));
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}

///
void precal()/// we apply the following operation for each v: cnt[link(v)] += cnt[v]
{
    set< pair<int,int> >::reverse_iterator it;
  for(  it=cal.rbegin(); it!=cal.rend(); it++ ){
       cnt[ st[ it -> second ].link ] += cnt[ it->second ];
       int len=it->first;
       ans[len]=max(ans[len],cnt[it->second]);
  }
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ///cout.tie(NULL);
    string s;
    cin>>s;
    sa_init();
    for(int i=0;i<s.length();i++)
    {
            sa_extend(s[i]-'a');
    }
    precal();
    /// ans[i] holds the maximum occurrence of longest string of length i
    /// so , if length of string i is occurred p times , so what is the max occurrence of string with length i-1
    /// it is obviously max(dp[i],dp[i-1]) ,right ?
    /// from backward take maximum and keep it in array of ans[]
    for(int i=s.length()-1;i>=0;i--){
    ans[i]=max(ans[i],ans[i+1]);
    }
    for(int i=1;i<=s.length();i++){
    cout<<ans[i]<<'\n';
    }
}
