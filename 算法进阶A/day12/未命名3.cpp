#include<bits/stdc++.h>
using namespace std;
#define int long long
const int N=1e6+5;

vector<int> a[N];
int dep[N],f[N],sz[N],son[N],top[N]; 
int n,m,rt,tot,P;
int v[N],seg[N],rev[N];
int sum[N<<2],tag[N<<2];
void dfs1(int u,int fa){//dep,f,sz,son
	dep[u]=dep[fa]+1,sz[u]=1,f[u]=fa;
	for(auto v:a[u]){
		if(v==fa) continue;
		dfs1(v,u);
		sz[u]+=sz[v];
		if(sz[son[u]]<sz[v]) son[u]=v;
	}
} 
void dfs2(int u,int tp){//top  
	// seg[i] 表示树上 i 号节点对应于线段树的下标； 
	// rev[i] 表示线段树下标 i 处对应于树上的哪个节点编号 
	top[u]=tp;
	if(son[u]){
		seg[son[u]]=++tot;
		rev[tot]=son[u];
		dfs2(son[u],tp);
	}
	for(auto v:a[u]){
		if(!top[v]){
			seg[v]=++tot;
			rev[tot]=v;
			dfs2(v,v);
		}
	}
}
int lca(int x,int y){ //跳到同一条重链 
	int fx=top[x],fy=top[y];
	while(fx!=fy){
		if(dep[fx]<dep[fy]) swap(x,y),swap(fx,fy);
		x=f[fx],fx=top[x];
	}	
	if(dep[x]<dep[y]) return x;
	else return y;
}
#define mid (l+r>>1)
#define lc (x<<1)
#define rc (x<<1|1)

void pushup(int x){
	sum[x]=sum[lc]+sum[rc],sum[x]%=P;
}

void pushdown(int x,int l,int r){
	if(tag[x]){
		tag[lc]+=tag[x],tag[lc]%=P;
		tag[rc]+=tag[x],tag[rc]%=P;
		sum[lc]+=(tag[x]*(mid-l+1))%P,sum[lc]%=P;
		sum[rc]+=(tag[x]*(r-mid))%P,sum[rc]%=P;
		tag[x]=0;	
	}
}
void build(int x,int l,int r){
	if(l==r){
		sum[x]=v[rev[l]]%P;
		return;
	}
	build(lc,l,mid);
	build(rc,mid+1,r);
	pushup(x);
}
void change(int x,int l,int r,int from,int to,int v){
	if(from<=l&&r<=to){
		tag[x]+=v,tag[x]%=P;
		sum[x]+=(v*(r-l+1))%P,sum[x]%=P;
		return; 
	}
	pushdown(x,l,r);
	if(from<=mid) change(lc,l,mid,from,to,v);
	if(to>mid) change(rc,mid+1,r,from,to,v);
	pushup(x); 
}
int query(int x,int l,int r,int from,int to){
	if(from<=l&&r<=to){
		return sum[x]%P;
	}
	pushdown(x,l,r);
	int res=0;
	if(from<=mid) res+=query(lc,l,mid,from,to),res%=P;
	if(to>mid) res+=query(rc,mid+1,r,from,to),res%=P;
	return res%P;
}
void path_change(int x,int y,int v){
	int fx=top[x],fy=top[y];
	while(fx!=fy){
		if(dep[fx]<dep[fy]) swap(x,y),swap(fx,fy);
		change(1,1,tot,seg[fx],seg[x],v);
		x=f[fx],fx=top[x];
	}
	if(dep[x]>dep[y]) swap(x,y);
	change(1,1,tot,seg[x],seg[y],v);
}
int path_query(int x,int y){
	int fx=top[x],fy=top[y],res=0;
	while(fx!=fy){
		if(dep[fx]<dep[fy]) swap(x,y),swap(fx,fy);
		res+=query(1,1,tot,seg[fx],seg[x]);
		res%=P; 
		x=f[fx],fx=top[x];
	}
	if(dep[x]>dep[y]) swap(x,y);
	res+=query(1,1,tot,seg[x],seg[y]);
	res%=P;
	return res;
}

signed main(){
	ios::sync_with_stdio(0);cin.tie(0),cout.tie(0);
	cin>>n>>m>>rt>>P;
	for(int i=1;i<=n;i++) cin>>v[i];
	for(int i=1;i<n;i++){
		int u,v;
		cin>>u>>v;
		a[u].push_back(v),a[v].push_back(u);	
	}
	dfs1(rt,0);
	seg[rt]=++tot,rev[tot]=rt,top[rt]=rt;
	dfs2(rt,rt);
	build(1,1,tot); 
	while(m--){
		int op,x,y,z;
		cin>>op;
		if(op==1){
			cin>>x>>y>>z;
			path_change(x,y,z);
		}
		if(op==2){
			cin>>x>>y;
			cout<<path_query(x,y)<<"\n";
		}
		if(op==3){
			cin>>x>>z;
			change(1,1,tot,seg[x],seg[x]+sz[x]-1,z);
		}
		if(op==4){
			cin>>x;
			cout<<query(1,1,tot,seg[x],seg[x]+sz[x]-1)<<"\n";
		}	
	}
	return 0;
}


