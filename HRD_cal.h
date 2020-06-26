#ifndef HRD_cal_H
#define HRD_cal_H

#include <vector>
#include <string>
using namespace std;

class HRD_cal {
    public:
        unsigned long long Change_int (char str[10]);
        string Change_str(unsigned long long dat);
        vector <unsigned long long> Calculate(unsigned long long Code);
        vector <unsigned long long> Calculate_All(unsigned long long Code);
        vector <unsigned long long> Calculate(unsigned long long Code, unsigned long long target);

    private:
        struct Case_cal {
            bool freeze[4][5]; // true -> no move ; false -> can move
            unsigned char status[4][5]; // 0xFF -> undefined ; 0xFE -> space
            unsigned char type[15]; // 0 -> 2 * 2 ; 1 -> 2 * 1 ; 2 -> 1 * 2 ; 3 -> 1 * 1
            unsigned long long code;
        };
        vector <Case_cal *> List; // ������ ����ÿ���ڵ����Ϣ
        vector <unsigned int> List_source; // ������һ����� ������Դ
        vector <unsigned long long> List_hash[0x10000]; // ��ϣ��
        unsigned int now_move; // ��ǰ���ڼ���Ŀ�ı��
        unsigned int result; // �õ���Ŀ����
        unsigned long long target_code;
        unsigned char mode; // 0 -> Calculate_All / 1 -> Calculate_Solution / 2 -> Calculate_Target
        bool flag; // �ж��Ƿ����ҵ�Ŀ��

        bool Parse_Code(Case_cal &dat, unsigned long long Code);
        void Get_Code(Case_cal &dat);
        void Find_Sub_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
        void Build_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
        void Find_Next_Case(Case_cal &dat_raw);
        void Add_Case(Case_cal *dat);
        void cal(unsigned long long Code);
        void init_data();
        vector <unsigned long long> Get_Path(unsigned int result_num);
};

#endif
