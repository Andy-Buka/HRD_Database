#ifndef HRD_analy_H
#define HRD_analy_H

#include <vector>
#include <string>
using namespace std;

class HRD_analy {
    public:
        struct Case_near;
        struct Case_cal {
            bool freeze[4][5]; // true -> no move ; false -> can move
            unsigned char status[4][5]; // 0xFF -> undefined ; 0xFE -> space
            unsigned char type[15]; // 0 -> 2 * 2 ; 1 -> 2 * 1 ; 2 -> 1 * 2 ; 3 -> 1 * 1
            unsigned long long code;
            unsigned int layer_num;
            unsigned int layer_index;
            Case_near *adjacent;
        };
        struct Case_near {
            vector <Case_cal *> source_case;
            vector <Case_cal *> next_case;
        };
        vector <vector <Case_cal *> > Layer; // ����ȫ�������ݵĽڵ�
        bool quiet = false; // true -> ��Ĭģʽ  false -> ����������
        // ���ֵĻ�������
        int min_solution_step; // ���ٵĲ���
        int min_solution_num; // ���ٲ���ĸ���
        vector <unsigned long long> min_solution_case; // �������ٲ���
        vector <unsigned int> solution_step; // ���н��Ӧ�Ĳ���
        int solution_num; // ��ĸ���
        vector <unsigned long long> solution_case; // ���н�
        int farthest_step; // ��Զ���ֵĲ���
        int farthest_num; // ��Զ���ֵĸ���
        vector <unsigned long long> farthest_case; // ������Զ�Ĳ���

        unsigned long long Change_int (char str[10]);
        string Change_str(unsigned long long dat);
        void Analyse_Case(unsigned long long code);
        void Output_Detail(string File_name);

    private:
        vector <Case_cal *> Layer_hash[0x10000]; // ��ϣ��
        Case_cal *now_move_case;
        unsigned int now_move_num, now_move_index; // ��ǰɨ��ڵ�Ĳ��� / ��ǰɨ��ڵ�Ĳ��б��

        bool Parse_Code(Case_cal &dat, unsigned long long Code);
        void Get_Code(Case_cal &dat);
        void Find_Sub_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
        void Build_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]);
        void Find_Next_Case(Case_cal &dat_raw);
        void Add_Case(Case_cal *dat);
        void Calculate(unsigned long long code);
        void Free_Data();
};

#endif
