
///Super Classical problem 
/// Time-0.40secs

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
    int len, link;
    map<int, int> next;
};

/// This should be declared globally
/// Number of states can be at most 2*n ,where n is the length of string
/// sz for pointer of number of states in SAM i.e. Suffix Automation

const int MAXLEN = 200010;
state st[MAXLEN * 2];
vector<int>occurrence_of_min[MAXLEN];///it will hold those state's min_length which occurs i times
vector<int>occurrence_of_max[MAXLEN];///it will hold those state's max_length which occurs i times

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

/// calculating occurrence of  strings of every state .
void precal()/// we apply the following operation for each v: cnt[link(v)] += cnt[v]
{
    set< pair<int,int> >::reverse_iterator it;
  for(  it=cal.rbegin(); it!=cal.rend(); it++ ){
       cnt[ st[ it -> second ].link ] += cnt[ it->second ];
  }
}

/// we know that if state (u) is the suffix link of state (v)
/// Then min(v)=len(u)+1 ..
/// where min(v) is the minimum length of strings of the state v...
/// By this relation,we can find the longest and minimum string's length of a state
/// It will help us to solve this problem

void min_length_cal()
{
    for(int i=0;i<sz;i++)
    {
        int max_length_of_i=st[i].len;
        int state_of_link_of_i=st[i].link;
        int min_length_of_i=st[state_of_link_of_i].len+1;
        int occurrence_of_state=cnt[i];
        occurrence_of_min[occurrence_of_state].push_back(min_length_of_i);
        occurrence_of_max[occurrence_of_state].push_back(max_length_of_i);
    }
    /// this sort will help us to do binary_search over here when queries will be inputted
    for(int i=0;i<=s.length();i++)
    {
        sort(occurrence_of_min[i].begin(),occurrence_of_min[i].end());
        sort(occurrence_of_max[i].begin(),occurrence_of_max[i].end());
    }
}

/// binary search over vector of a to get number of value less or equal than k ,takeing O(logn)
int not_greater (vector<int> &a, int k) {
	if (!a.size()) return 0;
	int l = 0, r = a.size() - 1, mid;
	while (l < r) {
		mid = (l + r + 1) / 2;
		if (a[mid] > k) r = mid - 1; else l = mid;
	}
	if (a[l] > k) return 0; else return l + 1;
}



int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ///cout.tie(NULL);
    cin>>s;
    sa_init();
    for(int i=0;i<s.length();i++)
    {
            sa_extend(s[i]-'a');
    }
    precal();
    min_length_cal();
    int q,len,times;
    cin>>q;
    while(q--)
    {
        cin>>len>>times;
    ///now first let's find how many min(i) is equal or less than len in vector in term of index of times
    ///count those state whose minlen(i) is equal or less than len ,those should be added because these contain length len obviously ,right ?
    /// But here is one problem , when maxlen(i) of some state is less than len ,those state should be subtracted from our collection , right ?
        int less_or_equal_than_len_of_min=not_greater(occurrence_of_min[times],len);
        int less_or_equal_than_len_of_max_minus_one=not_greater(occurrence_of_max[times],len-1);
        int ans=less_or_equal_than_len_of_min-less_or_equal_than_len_of_max_minus_one;
        cout<<ans<<'\n';
    }

    }
