/*
Test shell 要求完成的命令(Mandatory Commands)
-init
-cr <name> <priority>(=1 or 2) // create process
-de <name> // delete process
-req <resource name> <# of units> // request resource
-rel <resource name> <# of units> // release resource
-to // time out

查看进程状态和资源状态的命令
-list ready //list all processes in the ready queue
-list block // list all processes in the block queue
-list res //list all available resources

可选实现命令
-pr <name> //print pcb information about a given process.
*/
#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include <queue>

using namespace std;
struct RCB_list
{
  string rcb_id; // 进程使用的资源
  int used_resources;
  RCB_list *next;
};
//声明一个结构体类型 Process
struct PCB
{
  string pid;                  // 进程ID
  int cpu_state_message;       // 处理器状态信息
  int process_control_message; // 进程控制信息
  // 进程状态: ready/running/blocked <-> 0, 1, 2
  int process_state;              // 进程状态
  int process_priority;           // 进程优先级
  RCB_list *rcb_list;             // 占用的资源
  int user_stack;                 // 用户栈
  int private_user_address_space; // 私有用户地址空间
  int shared_address_space;       // 共享地址空间
};
struct Node
{
  // int RLid;
  PCB *pcb_data; //结点的数据
  Node *next;    //下一个结点的地址
};
struct RCB
{
  string rid;         // 资源ID
  int status;         // 空闲单元的数量
  Node *waiting_list; // 阻塞的进程列表
};

void init();
void create(string instruction);
void list_ready();
int get_process_priority_from_RL();
void to();
void request(string instruction);
void list_block();
void list_res();
void release(string instruction);

Node *ready_list[3];
RCB *resource_control_block[4];
map<string, int> resources_id_string_to_int;

int main()
{

  ifstream in("mock");
  cout << "HELLO" << endl;
  string instruction;
  while (getline(in, instruction))
  {
    cout << "Please enter your instructions: " << instruction << endl;
    if (instruction == "")
      break;
    if (instruction == "init")
    {
      init();
    }
    else if (instruction == "to")
    {
      to();
    }
    else if (instruction == "list ready")
    {
      list_ready();
    }
    else if (instruction == "list res")
    {
      list_res();
    }
    else if (instruction == "list block")
    {
      list_block();
    }
    //如果没找到，返回一个特别的标志c++中用npos表示
    else if (instruction.find("create") != instruction.npos)
    {
      create(instruction);
    }
    else if (instruction.find("request") != instruction.npos)
    {
      request(instruction);
    }
    else if (instruction.find("release") != instruction.npos)
    {
      release(instruction);
    }
    else
    {
      cout << "error" << endl;
    }
    cout << endl;
  }
  cout << "Bye" << endl;
  return 0;
}

void init()
{
  resources_id_string_to_int["R1"] = 1;
  resources_id_string_to_int["R2"] = 2;
  resources_id_string_to_int["R3"] = 3;
  resources_id_string_to_int["R4"] = 4;
  cout << "init process is running" << endl;

  // 具有 3 个优先级的就绪队列 RL 初始化;
  for (int i = 0; i < 3; i++)
  {
    ready_list[i] = new Node;
    ready_list[i]->next = nullptr;
  }

  // Init 进程;
  ready_list[0]->next = new Node;
  ready_list[0]->next->pcb_data = new PCB;
  ready_list[0]->next->pcb_data->pid = "init";
  ready_list[0]->next->pcb_data->process_state = 1;
  ready_list[0]->next->pcb_data->process_priority = 0;

  // 4 类资源,R1,R2,R3,R4,每类资源 Ri 有 i 个
  for (int i = 1; i < 5; i++)
  {
    resource_control_block[i] = new RCB;
    resource_control_block[i]->rid = "R" + to_string(i);
    resource_control_block[i]->status = i;
    resource_control_block[i]->waiting_list = new Node;
    resource_control_block[i]->waiting_list->next = nullptr;
  }
}

void to()
{
  int running_process_priority = get_process_priority_from_RL();
  Node *pHead = ready_list[running_process_priority];
  if (pHead->next->next != nullptr)
  {
    Node *new_running_process_node = pHead->next->next;
    new_running_process_node->pcb_data->process_state = 1;
    ready_list[running_process_priority]->next->pcb_data->process_state = 0;
    while (pHead->next != nullptr)
    {
      pHead = pHead->next;
    }
    // 把新之前运行的进程放到相应的就绪队列的末尾
    pHead->next = ready_list[running_process_priority]->next;
    ready_list[running_process_priority]->next = new_running_process_node;
    pHead->next->next = nullptr;
    // pHead->next->pcb_data->pid = process_id;
  }
  cout << "process " << ready_list[running_process_priority]->next->pcb_data->pid << " is running.";
  ready_list[running_process_priority]->next->next &&cout << " process " << ready_list[running_process_priority]->next->next->pcb_data->pid << " is ready";
  cout << endl;
}

void list_ready()
{
  string running_process_id;
  for (int i = 2; i >= 0; i--)
  {
    cout << i << ": ";
    Node *pHead = ready_list[i];
    if (pHead->next && pHead->next->pcb_data->process_state == 1)
    {
      running_process_id = pHead->next->pcb_data->pid;
    }
    while (pHead->next != nullptr)
    {
      cout << pHead->next->pcb_data->pid << " (" << pHead->next->pcb_data->process_state << ") ";
      pHead->next->next &&cout << " -> ";
      pHead = pHead->next;
    }
    cout << endl;
  }

  cout << "process " << running_process_id << " is running" << endl;
}

void create(string instruction)
{
  // 提取指令中的 pid 和 priority
  string create_instruction = instruction.substr(7);
  int space_index = create_instruction.find(" ");
  string process_id = create_instruction.substr(0, space_index);
  int process_priority = stoi(create_instruction.substr(space_index + 1));

  int running_process_priority = get_process_priority_from_RL();

  if (ready_list[running_process_priority]->next->pcb_data->process_state == 3)
  {
    cout << "3" << endl;
    ready_list[process_priority]->next->pcb_data->process_state = 1;
    ready_list[process_priority]->next->pcb_data->pid = process_id;
  }
  else
  {
    // 当前正在运行的进程优先级高于或者等于新创建的进程
    if (running_process_priority >= process_priority)
    {
      // cout << "当前正在运行的进程优先级高于或者等于新创建的进程" << endl;
      Node *pHead = ready_list[process_priority];
      while (pHead->next != nullptr)
      {
        pHead = pHead->next;
      }
      // 把新的进程放到相应的就绪队列的末尾
      pHead->next = new Node;
      pHead->next->pcb_data = new PCB;
      pHead->next->next = nullptr;
      pHead->next->pcb_data->pid = process_id;
      pHead->next->pcb_data->process_state = 0;
      pHead->next->pcb_data->process_priority = process_priority;
      pHead->next->pcb_data->rcb_list = new RCB_list;
      pHead->next->pcb_data->rcb_list->used_resources = 0;
      pHead->next->pcb_data->rcb_list->next = nullptr;

      // 因为正在运行的进程依然是之前运行的进程, 所以把 running_process 的值赋给 process_id
      process_id = ready_list[running_process_priority]->next->pcb_data->pid;
    }
    // 当前正在运行的进程优先级低于新创建的进程, 运行新创建的进程,block原来运行的进程
    else
    {
      // cout << "block 原先的进程, 把它放到原来就绪队列的末尾, 并设置状态为 block" << endl;
      // block 原先的进程, 把它放到原来就绪队列的末尾, 并设置状态为 block

      Node *running_process_node = ready_list[running_process_priority]->next;
      if (ready_list[running_process_priority]->next->next == nullptr)
      {
        running_process_node->pcb_data->process_state = 0;
      }
      else
      {
        Node *pHead = ready_list[running_process_priority]->next;
        Node *running_process_next_process = ready_list[running_process_priority]->next->next;

        while (pHead->next != nullptr)
        {
          pHead = pHead->next;
        }
        ready_list[running_process_priority]->next = running_process_next_process;
        pHead->next = running_process_node;
        running_process_node->next = nullptr;
        running_process_node->pcb_data->process_state = 0;
      }

      // 把新的运行的进程放到相应优先级队列的头部
      ready_list[process_priority]->next = new Node;
      ready_list[process_priority]->next->pcb_data = new PCB;
      ready_list[process_priority]->next->pcb_data->process_state = 1;
      ready_list[process_priority]->next->pcb_data->pid = process_id;
      ready_list[process_priority]->next->pcb_data->process_priority = process_priority;
      ready_list[process_priority]->next->pcb_data->rcb_list = new RCB_list;
      ready_list[process_priority]->next->pcb_data->rcb_list->next = nullptr;
      ready_list[process_priority]->next->next = nullptr;
    }
  }

  cout << "process " << process_id << " is running" << endl;
}

void request(string instruction)
{
  string create_instruction = instruction.substr(8);
  int space_index = create_instruction.find(" ");
  string resources_id_string = create_instruction.substr(0, space_index);
  int resources_status = stoi(create_instruction.substr(space_index + 1));

  RCB *current_request_resources = resource_control_block[resources_id_string_to_int[resources_id_string]];
  int running_process_priority = get_process_priority_from_RL();
  Node *running_process = ready_list[running_process_priority]->next;
  Node *waiting_list_ptr = current_request_resources->waiting_list;
  if (resources_status > current_request_resources->status)
  {
    // 资源不够, 申请资源的进程被阻塞
    ready_list[running_process_priority]->next = ready_list[running_process_priority]->next->next;
    while (waiting_list_ptr->next)
    {
      waiting_list_ptr = waiting_list_ptr->next;
    }
    cout << current_request_resources->rid << endl;
    cout << running_process->pcb_data->pid;
    running_process->pcb_data->process_state = 2;
    waiting_list_ptr->next = running_process;
    waiting_list_ptr->next->next = nullptr;

    ready_list[running_process_priority]->next->pcb_data->process_state = 1;
  }
  else
  {
    current_request_resources->status -= resources_status;
    running_process->pcb_data->rcb_list->used_resources = resources_status;
    RCB_list *running_process_use_resources = running_process->pcb_data->rcb_list;
    while (running_process_use_resources->next != nullptr)
    {
      running_process_use_resources = running_process_use_resources->next;
    }
    running_process_use_resources->rcb_id = current_request_resources->rid;
    running_process_use_resources->next = nullptr;
    cout << "process " << running_process->pcb_data->pid << " requests " << resources_status << " " << resources_id_string << endl;
  }
}

void release(string instruction)
{
  string release_pid = instruction.substr(8);
  int use_resources_nums = 0;
  PCB *will_delete_process;

  // 从 ready list 队列中寻找进程
  for (int i = 2; i >= 0; i--)
  {
    Node *pHead = ready_list[i]->next;
    while (pHead)
    {
      if (pHead->pcb_data->pid == release_pid)
      {
        will_delete_process = pHead->pcb_data;
      }
      pHead = pHead->next;
    }
  }
  // 从 block list 中寻找进程
  for (int i = 1; i < 5; i++)
  {
    Node *waiting_list_ptr = resource_control_block[i]->waiting_list->next;
    while (waiting_list_ptr)
    {
      if (waiting_list_ptr->pcb_data->pid == release_pid)
      {
        will_delete_process = waiting_list_ptr->pcb_data;
      }
      waiting_list_ptr = waiting_list_ptr->next;
    }
  }
  cout << "release ";

  RCB_list *will_delete_process_use_rcb_list_ptr = will_delete_process->rcb_list;

  while (will_delete_process_use_rcb_list_ptr)
  {
    // 释放该进程使用的资源
    resource_control_block[resources_id_string_to_int[will_delete_process_use_rcb_list_ptr->rcb_id]]->status += will_delete_process_use_rcb_list_ptr->used_resources;
    cout << will_delete_process_use_rcb_list_ptr->rcb_id << " ";

    Node *next_process = resource_control_block[resources_id_string_to_int[will_delete_process_use_rcb_list_ptr->rcb_id]]->waiting_list->next;

    if (next_process == nullptr)
    {
      // 说明待删除进程使用的资源的阻塞队列中没有被阻塞的进程
    }
    else
    {
      PCB *block_process = next_process->pcb_data;

      if (true)
      {
        // 说明该进程可以被放到就绪队列中
        resource_control_block[resources_id_string_to_int[will_delete_process_use_rcb_list_ptr->rcb_id]]->waiting_list = new Node;
        resource_control_block[resources_id_string_to_int[will_delete_process_use_rcb_list_ptr->rcb_id]]->waiting_list->next = next_process->next;
        Node *pHead = ready_list[next_process->pcb_data->process_priority]->next;

        while (pHead->next != nullptr)
        {
          pHead = pHead->next;
        }
        pHead->next = next_process;
        pHead->next->next = nullptr;
      }
    }
    will_delete_process_use_rcb_list_ptr = will_delete_process_use_rcb_list_ptr->next;
    cout << ". wake up process ";
    cout << endl;
  }
  // 然后直接删除该进程, 该进程可能存在与阻塞队列中或者就绪队列中
  // TODO
}

int get_process_priority_from_RL()
{
  for (int i = 2; i >= 0; i--)
  {
    if (ready_list[i]->next && ready_list[i]->next->pcb_data->process_state == 1)
    {
      return i;
    }
  }
}
void list_block()
{
  for (int i = 1; i < 5; i++)
  {
    cout << resource_control_block[i]->rid << ":" << resource_control_block[i]->status << " ";
    Node *waiting_list_ptr = resource_control_block[i]->waiting_list->next;
    while (waiting_list_ptr != nullptr)
    {
      cout << waiting_list_ptr->pcb_data->pid << " ";
      waiting_list_ptr = waiting_list_ptr->next;
    }
    cout << endl;
  }
}
void list_res()
{
  for (int i = 1; i < 5; i++)
  {
    cout << resource_control_block[i]->rid << ": " << resource_control_block[i]->status << endl;
  }
}