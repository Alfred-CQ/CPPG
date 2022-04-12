#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

#define INF 1000000000
typedef long long int ll;

using namespace std;

class STree
{
    public:
        vector<int> m_array;
        vector<ll> m_tree;
        vector<ll> m_lazy;

        int sizeArr, sizeTree;

        STree(vector<int>&);

        void build();
        void build(int,int,int);

        //void update(int, int);
        //void update(int, int, int, int, int);

        void updateRange(int, int, int);
        void updateRange(int, int, int, int, int, int);

        ll query(int, int);
        ll query(int, int, int, int, int);

};

STree::STree(vector<int>& array)
{
    sizeArr = array.size(); sizeTree = (sizeArr * 4); 

    m_array = array;
    m_tree = vector<ll>(sizeTree,INF);
    m_lazy = vector<ll>(sizeTree,0);
    build();
}

void STree::build()
{
    build(0,sizeArr - 1, 1);
}

void STree::build(int beg, int end, int set)
{
    m_tree[set] = INF;
    if (beg == end)
        m_tree[set] = m_array[beg];
    else 
    {
        int mid = (beg + end) / 2;
        build(beg, mid,  set * 2);
        build(mid + 1, end, (set * 2) + 1);
        m_tree[set] = min(m_tree[set * 2], m_tree[set * 2 + 1]);
    }
}

ll STree::query(int i, int j)
{
    return query(i,j, 0, sizeArr-1 , 1);
}

ll STree::query(int i, int j, int beg, int end, int set)
{   
    if (m_lazy[set] != 0)
    {
        m_tree[set] += m_lazy[set];
        if (beg != end)
        {
            m_lazy[set*2] += m_lazy[set];
            m_lazy[set*2+1] += m_lazy[set];
        }
        m_lazy[set] = 0;
    }

    if (end < i || beg > j || beg > end)
        return INF;

    if (i <= beg && j >= end)
        return m_tree[set];

    
    int mid = (beg + end) / 2;

    ll a = query(i, j, beg, mid, set*2);
    ll b = query(i, j,mid + 1, end, (set * 2) + 1);

    return min(a,b);
}

void STree::updateRange(int i, int j, int k)
{
    updateRange(0, sizeArr-1 , 1 , i, j, k);
}

void STree::updateRange(int beg, int end, int set, int i, int j, int k)
{   
    
    if (m_lazy[set] != 0)
    {
        m_tree[set] += m_lazy[set];
 
        if (beg != end)
        {
            m_lazy[set*2]   += m_lazy[set];
            m_lazy[set*2 + 1]   += m_lazy[set];
        }

        m_lazy[set] = 0;
    }
 
    if (beg>j || end<i || beg > end)
        return ;

    if (beg>=i && end<=j)
    {
        m_tree[set] += k;

        if (beg != end)
        {
            m_lazy[set*2]   += k;
            m_lazy[set*2 + 1]   += k;
        }
        return;
    }
 
    int mid = (beg+end)/2;
    updateRange(beg, mid, set*2, i, j, k);
    updateRange(mid+1, end, set*2+1, i, j, k);
 
    m_tree[set] = min(m_tree[set*2],m_tree[set*2+1]);
}

int main() {
	int n, m, lf, rg, v;
    int i, k;
    
    cin >> n;

    vector<int> myVector(n);

    for (i = 0; i < n; ++i)
        cin >> myVector[i];

    cin >> m;

    STree mySTree(myVector);

    while (m>=0)
    {
        m--;
        string str;
        vector<int> values;

        getline(cin, str);
		stringstream ss(str);

		while (ss >> k)
			values.push_back(k);
		
		if (values.size() == 0)
			continue;

        lf = values[0];
		rg = values[1];

        if (values.size() == 3)
        {
            v = values[2];
            if(lf <= rg)
		mySTree.updateRange(lf, rg, v);
	    else
	    {
		mySTree.updateRange(lf, n-1, v);
		mySTree.updateRange(0,rg, v);
	    }
        }
        else
        {
            if (lf <=  rg)
		cout << mySTree.query(lf, rg) << endl;
	    else
		cout << min(mySTree.query(lf, n-1), mySTree.query(0, rg)) << endl;
				
        }
        values.clear();
    }

    return 0;
}
