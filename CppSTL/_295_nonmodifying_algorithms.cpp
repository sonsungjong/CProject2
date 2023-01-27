// nonmodifying algorithms : 원소를 수정하지 않는 알고리즘, 순서나 값을 변경하지 않고 읽기만 하는 알고리즘

/*

p = adjacent_find(begin, end) : p는 [begin, end) 원소 중 *p == *(p+1)인 첫 원소를 가리키는 반복자
p = adjacent_find(begin, end, f) : p는 [begin, end) 원소 중 f(*p, *(p+1)) 이 참인 첫 원소를 가리키는 반복자
n = count(begin, end, x) : n은 구간 [begin, end) 원소 중 x 원소의 갯수
n = count_if(begin, end, f) : n은 구간 [begin, end) 원소 중 f(*p)가 참인 원소의 갯수
equal(begin, end, b2) : [begin, end)와 [b2, b2+(end-begin))의 모든 원소가 같은가?
equal(begin, end, b2, f) : [begin, end)와 [b2, b2+(e-b))의 모든 원소가 f(*p, *q)가 참인가?
p = find(b, e, x) : p는 구간 [b, e)에서 x와 같은 첫 원소의 반복자
p = find_end(b, e, b2, e2) : p는 구간 [b, e)의 순차열 중 구간 [b2, e2)의 순차열과 일치하는 순차열 첫 원소의 반복자, 단, [b2, e2)와 일치하는 순차열이 여러개라면 마지막 순차열 첫 원소의 반복자
p = find_end(b, e, b2, e2, f) : p는 구간 [b, e)의 순차열 중 구간 [b2, e2)의 순차열과 일치하는 순차열 첫 원소의 반복자. 단, [b2, e2)와 일치하는 순차열이 여러개라면 마지막 순차열 첫 원소의 반복자. 비교는 f를 사용
p = find_first_of(b,e,b2,e2) : p는 구간 [b,e)에서 구간 [b2,e2)의 원소 중 같은 우너소가 발견된 첫 원소의 반복자
p = find_first_of(b,e,b2,e2,f) : p는 구간 [b,e)에서 구간 [b2,e2)의 원소 중 같은 원소가 발견된 첫 원소의 반복자. 비교는 f를 사용
p = find_if(b,e,f) : p는 구간 [b,e)에 f(*p)가 참인 첫 원소를 가리키는 반복자
f = for_each(b,e,f) 구간 [b,e)의 모든 원소에 f(*p) 동작을 적용한다. f를 다시 돌려준다
lexicographical_compare(b,e,b2,e2) : 구간 [b,e)의 순차열이 구간 [b2,e2)의 순차열보다 작다면(less) true, 아니면 false를 반환. 작음은 사전순
lexicographical_compare(b,e,b2,e2,f) : 구간 [b,e)의 순차열이 구간 [b2,e2)의 순차열보다 작다면(less) true, 아니면 false를 반환. 작음은 [b,e)의 반복자 p와 [b2, e2)의 반복자 q에 대해 f(*p, *q)가 참이다.
k = max(a,b) : k는 a 와 b 중 큰 것
k = max(a,b,f) : k는 a와 b 중 큰 것, 이때 큰 것은 f(a,b)를 사용
p = max_element(b,e) : p는 구간 [b,e)에서 가장 큰 원소의 반복자
p = max_element(b, e, f) : p는 구간 [b,e)에서 가장 큰 원소의 반복자. 비교는 f를 사용
k = min(a,b) : k는 a와 b 중 작은 것
k = min(a,b,f) : k는 a와 b 중 작은 것. 이때 작은 것은 f(a,b)를 사용
p = min_element(b,e) : p는 구간 [b,e)에서 가장 작은 원소의 반복자
p = min_element(b,e,f) : p는 구간 [b,e)에서 가장 작은 원소의 반복자. 비교는 f를 사용
pair(p,q) = mismatch(b,e,b2) : (p,q)는 구간 [b,e)와 [b2, b2+(e-b))에서 !(*p==*q)인 첫 원소를 가리키는 반복자의 쌍
pair(p,q) = mismatch(b,e,b2,f) : (p,q)는 구간 [b,e)와 [b2, b2+(e-b))에서 !f(*p, *q)가 참인 첫 원소를 가리키는 반복자의 쌍
p = search(b,e,b2,e2) : p는 구간 [b,e)의 순차열 중 구간[b2,e2)의 순차열과 일치하는 순차열 첫 원소의 반복자(find_end()와 비슷하나 find_end()는 일치하는 순차열의 마지막 순차열의 반복자)
p = search(b,e,b2,e2,f) : p는 구간 [b,e)의 순차열 중 구간 [b2,e2)의 순차열과 일치하는 순차열 첫 원소의 반복자. 이때 비교는 f 를 사용
p = search_n(b,e,n,x) : p는 구간 [b,e)의 원소 중 x값이 n개 연속한 첫 원소의 반복자
p = search_n(b,e,n,x,f) : p는 구간 [b,e)의 원소 중 f(*p, x)가 참인 값이 n개 연속한 첫 원소의 반복자

*/
