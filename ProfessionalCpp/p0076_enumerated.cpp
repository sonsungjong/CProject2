import "HeaderUnits.h";

// enum class
enum class PieceType : unsigned int
{
	King = 1,
	Queen,
	Rook = 10,
	Pawn
};

// ���� ����� enum ���ٴ� enum class�� ����ϴ� ���� ���� (Ÿ�� ������)
enum class State{Unknown, Started, Finished};
enum class Error{None, BadInput, DiskFull, Unknown};

// ���� ����� enum
enum OldPiece {PieceTypeKing, PieceTypeQueen, PieceTypeRook, PieceTypePawn};

//bool ok = false ;						// error C2365: 'ok': redefinition; previous definition was 'data variable'
//enum Status { error, ok };				// ������� ���� enum�� �� �̸��� ��ġ�� ������ �߻��Ѵ�

int main()
{
	// ��Ÿ���� enum class
	PieceType piece{ PieceType::King };

	// ���� ����� enum
	OldPiece myPiece{ PieceTypeQueen };				// enum�� ���̸��� ������ ��ġ�� �ʰ� ����������Ѵ�

}