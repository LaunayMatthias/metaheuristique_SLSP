class SLSP{
public:
  int _T;
  int _P;
  int _W;

  SLSP(int t):_T(t), _P(t/2), _W(t-1){
    std::cout << _T << '\n';
  }
};
