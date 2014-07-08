#define ButtonHistory_h

class ButtonHistory{
  public:
  	ButtonHistory(void);
  	int add(int);
  	bool show(void);
  	short *get_all(void);
  	short get_size(void);
  	short get_by_index(int);

  private:
  	short size;
  	short history[];
  	short head;
  	short tail;
};