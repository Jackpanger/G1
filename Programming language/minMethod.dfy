method min(a:array<int>) returns (res:int,p:int)
    requires a.Length > 0
    ensures forall i: int::0<=i<a.Length ==>a[i]>=res
    ensures 0<=p<a.Length && a[p] == res
    {
        res:=a[0];
        p:=0;
        var i:1;
        while(i<a.Length)
            invariant 0<i<=a.Length
            invariant res forall i: int::0<=i<a.Length ==>a[i]>=res
            invariant 0<=p<a.Length && a[p] == res
        {
            if(a[i]<res){
                res:=a[i];
                p:=i;
            }
            i:=i+1;
        }
    }

method main(){
    print("hello word")
}