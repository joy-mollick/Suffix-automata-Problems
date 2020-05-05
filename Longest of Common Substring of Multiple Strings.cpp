

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
    int len, link ;
    map<int, int> next;
};

/// This should be declared globally
/// Number of states can be at most 2*n ,where n is the length of string
/// sz for pointer of number of states in SAM i.e. Suffix Automation

const int MAXLEN = 100001;
int dp[12][2*MAXLEN];///dp[i][j] , it will hold the longest common substring of state j of first string with ith string
state st[MAXLEN * 2];
int sz, last;
/// First state is the first one ,so len is zero
/// There is no suffix link of first state ,so link is -1
/// last is obviously zero for first state

void sa_init() {
   /// for (int i = 0; i <=sz; i++)
    ///   st[i].next.clear();

	sz = last = 0;
	st[0].len = 0;
	st[0].link = -1;
	++sz;
}

/// when we add new character then we will check by last pointer to which label with our new character c

void sa_extend(int c) {
    int cur = sz++;/// increase the size of sz
    st[cur].len = st[last].len + 1;/// it's obviously increasing by one as new charcter is adding
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
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}

void longest_commn_sbstrng(int caso,string t)
{
    int state_num=0,match=0;

    /// state_num holds for node number where longest string remains which is also suffix of prefix ending at i
    /// match is current matching with suffix ending with i
    /// length_of_lngst_mtch is amongst all of the match
    /// best_match_ending_point states the index of prefix of t whose suffix with length of length_of_lngst_mtch.

    for(int i=0;i<t.length();i++)

    {

    while(state_num && !st[state_num].next.count(t[i]-'a'))
    {
        state_num=st[state_num].link;
        match=st[state_num].len;
    }

    if(st[state_num].next.count(t[i]-'a'))
    {
        state_num=st[state_num].next[t[i]-'a'];
        match++;
    }

    dp[caso][state_num]=max(dp[caso][state_num],match);/// take maximum one ,like maximum substring between two strings

    }

    for(int state=sz-1;state>=0;state--)
    {
        if(dp[caso][state])/// not equal zero ,then enter into this
        {
            while(state)///not equal to zero i.e. root state
            {
                state=st[state].link;
                dp[caso][state]=st[state].len;
            }
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    string s;
    cin>>s;
    sa_init();
    for(int i=0;i<s.length();i++)
    {
            sa_extend(s[i]-'a');
    }
    for(int j=1;j<sz;j++) dp[0][j]=st[j].len;/// precompute for the first string ,for all states
    int caso=1;
    while(cin>>s)
    {
        longest_commn_sbstrng(caso,s);
        caso++;
    }

    /// now dp[i][j] for all ith string's common string with first string for every state has been calculated
    /// now for each state compute the min among all string because it is common to all strings ,take thsi for each state and keep maximum of them

    int res=0;
    for(int state=1;state<sz;state++)
    {
        int temp=dp[0][state];/// take this temp as reference of comparison
        for(int i=1;i<caso;i++)
        {
            temp=min(temp,dp[i][state]);/// keep minimum for all strings of this strings of state of all strings by comparing with the first string ,because minimum is common to all
        }
        res=max(temp,res);
    }
    cout<<res<<'\n';
}
