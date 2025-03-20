import xlrd
import pandas as pd
import sys

# xls_file = "C:\\Users\\Administrator\\source\\repos\\CProject2\\exe\\유도탄_천궁15A017-00_확인_202503201049.xls"
# df = pd.read_excel(xls_file, engine='xlrd')

# # CSV 형식으로 변환 후 표준 출력 (쉼표 `,` 로 구분)
# csv_writer = csv.writer(sys.stdout, quoting=csv.QUOTE_MINIMAL)
# csv_writer.writerows(df.values.tolist())

def read_xls(filepath):
    df = pd.read_excel(filepath, engine='xlrd', header=None)

    for r in range(df.shape[0]):
        for c in range(df.shape[1]):
            value = str(df.iloc[r, c])  # 값을 문자열로 변환
            if value == "nan" or value.strip() == "":  # 빈 값은 무시
                continue  
            print(f"{r+1} {c+1} {value}")  # `row col value` 형식으로 출력

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("ERROR: No file path provided.")
        sys.exit(1)
    xls_file = sys.argv[1]          # C++에서 전달된 첫번째 인수 사용
    read_xls(xls_file)