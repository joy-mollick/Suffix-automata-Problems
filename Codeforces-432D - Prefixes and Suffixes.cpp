
/// Time-0.124s
/// Very Nice One 

#include<bits/stdc++.h>

using namespace std;


string s;
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

const int MAXLEN = 100001;
vector< pair<int,int> >ans;
state st[MAXLEN * 2];
int cnt[2*MAXLEN]; ///It holds for the size of each endpos set of occurrence of each state ,because each state corresponds to an endpos set of all occurrence of those string in that set ,so we need that size
int sz, last;
set< pair<int,int> > cal;/// It is for aid of calculation of each state endpos set class size,it will be sorted according to the size of long suffix of a state

/// First state is the first one ,so len is zero
/// There is no suffix link of first state ,so link is -1
/// last is obviously zero for first state

void sa_init() {
  ///  for (int i = 0; i <=sz; i++)
  ///      st[i].next.clear();

	sz = last = 0;
	st[0].len = 0;
	st[0].link = -1;
	++sz;
}

/// when we add new character then we will check by last pointer to which label with our new character c

void sa_extend(int c) {
    int cur = sz++;/// increase the size of sz
    st[cur].len = st[last].len + 1;/// it's obviously increasing by one as new charcter is adding
    st[cur].ending_pos=st[cur].len;/// we will consider 1 based indexing here ,so that it can be actually length of prefix also
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
            st[clone].ending_pos=st[q].ending_pos;
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

/// calculating occurrence of  strings of every state .
void precal()
{
    set< pair<int,int> >::reverse_iterator it;
  for(  it=cal.rbegin(); it!=cal.rend(); it++ ){
       cnt[ st[ it -> second ].link ] += cnt[ it->second ];
  }
}

/// carefully check the all suffixes of whole string
/// because all suffixes of whole string is also suffix of string
/// then check that ending_position like i.e. first occurrence
/// then check that is ending_pos of a state is also length of longest suffix of that state
/// then it is very obvious that is also prefix as well as suffix
/// find the occurrence of that state i.e. cont[state]
/// that is all. Nothing else .
void solve()
{
    int len;
    while(last)/// until we reach root node i.e. last is zero(0)
    {
        len=st[last].len;/// it is length of longest suffix of last state
        if(len==st[last].ending_pos)
        {
            ans.push_back(make_pair(len,cnt[last]));
        }
        last=st[last].link;/// take link of state of longest suffix of current one.
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin>>s;
    sa_init();
    for(int i=0;i<s.length();i++)
    {
            sa_extend(s[i]-'a');
    }
    precal();
    solve();
    int siz=ans.size();
    cout<<siz<<'\n';
    /// print from last iterator because , it is said that list of this should be increasing order
    for(int i=siz-1;i>=0;i--) cout<<ans[i].first<<" "<<ans[i].second<<'\n';
    }
