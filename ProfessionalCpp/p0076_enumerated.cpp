import "HeaderUnits.h";

// enum class
enum class PieceType : unsigned int
{
	King = 1,
	Queen,
	Rook = 10,
	Pawn
};

// 예전 방식의 enum 보다는 enum class를 사용하는 것이 좋다 (타입 안정성)
enum class State{Unknown, Started, Finished};
enum class Error{None, BadInput, DiskFull, Unknown};

// 예전 방식의 enum
enum OldPiece {PieceTypeKing, PieceTypeQueen, PieceTypeRook, PieceTypePawn};

//bool ok = false ;						// error C2365: 'ok': redefinition; previous definition was 'data variable'
//enum Status { error, ok };				// 변수명과 예전 enum의 값 이름이 곂치면 에러가 발생한다

int main()
{
	// 강타입인 enum class
	PieceType piece{ PieceType::King };

	// 예전 방식의 enum
	OldPiece myPiece{ PieceTypeQueen };				// enum의 값이름이 변수와 곂치지 않게 조심해줘야한다

}