#include <bits/stdc++.h>

using namespace std;
/*

FCFS:
input =>{at,bt,p},id;
1. sort by at.
2. complete burst time.
3. stores first time enters in the CPU, completion time
*/

void FCFS(vector < pair < vector < int > , int >> v, map < int, vector < pair < int, int >>> & mp) {
    sort(v.begin(), v.end());
    int curr = v[0].first[0];
    for (int i = 0; i < v.size(); i++) {
        int id = v[i].second;
        int at = v[i].first[0];
        int bt = v[i].first[1];
        curr = max(curr, at);
        mp[id].push_back({
            curr,
            curr + bt
        });
        curr += bt;
    }
}
/*
SJF:
1.Sort arrival time, if same then minimum burst time first
*/
bool SJF_comp(const pair < vector < int > , int > & a,
    const pair < vector < int > , int > & b) {
    if (a.first[1] != b.first[1]) return a.first[1] < b.first[1];
    if (a.first[0] != b.first[0]) return a.first[0] < b.first[0];
    return a.second < b.second;
}
void SJF(vector < pair < vector < int > , int >> v, map < int, vector < pair < int, int >>> & mp) {
    sort(v.begin(), v.end(), SJF_comp);
    int curr = v[0].first[0];
    for (int i = 0; i < v.size(); i++) {
        int id = v[i].second;
        int at = v[i].first[0];
        int bt = v[i].first[1];
        curr = max(curr, at);
        mp[id].push_back({
            curr,
            curr + bt
        });
        curr += bt;
    }
}
/*
Non-premptive Priority:
1. Low values means higher priority assumption
2. Now higher priority based processes will burst first.
*/
bool priority_cmp(const pair < vector < int > , int > & a,
    const pair < vector < int > , int > & b) {
    if (a.first[2] != b.first[2]) return a.first[2] < b.first[2];
    if (a.first[0] != b.first[0]) return a.first[0] < b.first[0];
    return a.second < b.second;
}
void priority_non_premptive(vector < pair < vector < int > , int >> v, map < int, vector < pair < int, int >>> & mp) {
    sort(v.begin(), v.end(), priority_cmp);
    int curr = v[0].first[0];
    for (int i = 0; i < v.size(); i++) {
        int id = v[i].second;
        int at = v[i].first[0];
        int bt = v[i].first[1];
        curr = max(curr, at);
        mp[id].push_back({
            curr,
            curr + bt
        });
        curr += bt;
    }
}
/*
SRJF:
1. SJF with premptive
2. Cpu will first start the process with earliet arrival time
3. then its executes the process for unit time, at that current time if any other process with minimum burst time arrives it will be executed.
4. In this case wating time!= Response time
5. After, once all processes are inserted into the reday queue, a SJF algorithm will be performed.
I am storing their presence intervals in the CPU.
1-> 0 1, 1 2, 4 5.... why 1 second as it is the time quantum.
*/
void SRJF(vector < pair < vector < int > , int >> & v, map < int, vector < pair < int, int >>> & mp) {
    int n = v.size();
    set < pair < vector < int > , int > , decltype( & SJF_comp) > ready(SJF_comp);
    vector < bool > inserted(n, false);
    vector < int > remaining(n);
    int completed = 0, time = 0;

    for (int i = 0; i < n; ++i)
        remaining[i] = v[i].first[1];

    while (completed < n) {
        for (int i = 0; i < n; ++i) {
            if (!inserted[i] && v[i].first[0] <= time) {
                ready.insert({
                    {
                        v[i].first[0], remaining[i], v[i].first[2]
                    },
                    i
                });
                inserted[i] = true;
            }
        }

        if (!ready.empty()) {
            auto it = ready.begin();
            auto[vec, pid] = * it;
            ready.erase(it);

            mp[pid].push_back({
                time,
                time + 1
            });
            remaining[pid]--;
            time++;

            if (remaining[pid] > 0) {
                ready.insert({
                    {
                        time,
                        remaining[pid],
                        vec[2]
                    },
                    pid
                });
            } else {
                completed++;
            }
        } else {
            time++;
        }
    }
}
/*
Robin Round:
1. First entry according to the arrrival time in the ready queue for the first time
2. then pop it from the ready queue, then execute it in the CPU for the tq.
3. During that time other processes enters the ready queue according to the arrival time
4. After execution if the burst time reamins , the process added at the end of the ready queue.
5. This continues till the all processes exhausted.
6. It can never be non-premptive
*/
void Robin_Round(vector < pair < vector < int > , int >> v, map < int, vector < pair < int, int >>> & mp, int tq) {
    queue < pair < vector < int > , int >> ready;
    set < pair < vector < int > , int >> st(v.begin(), v.end());
    auto x = st.begin();
    ready.push({
        * x
    });
    int curr = ( * st.begin()).first[0];
    st.erase(x);
    while (!ready.empty()) {
        auto z = ready.front();
        int id = z.second;
        int at = z.first[0];
        int bt = z.first[1];
        ready.pop();
        curr = max(curr, at);
        mp[id].push_back({
            curr,
            curr + min(tq, bt)
        });
        curr += min(tq, bt);
        while (!st.empty() && ((curr >= ( * st.begin()).first[0]))) {
            ready.push( * st.begin());
            st.erase(st.begin());
        }
        if (bt >= tq) bt -= tq;
        else bt = 0;
        if (bt > 0) ready.push({
            {
                at,
                bt,
                z.first[2]
            },
            id
        });
    }
}
/*
Priority_premptive:
same as SRJF but the criteria here is the priority.
*/
void priority_premptive(vector < pair < vector < int > , int >> v, map < int, vector < pair < int, int >>> & mp) {
    int n = v.size();
    set < pair < vector < int > , int > , decltype( & priority_cmp) > ready(priority_cmp);
    vector < bool > inserted(n, false);
    vector < int > remaining(n);
    int completed = 0, time = 0;

    for (int i = 0; i < n; ++i)
        remaining[i] = v[i].first[1];

    while (completed < n) {
        for (int i = 0; i < n; ++i) {
            if (!inserted[i] && v[i].first[0] <= time) {
                ready.insert({
                    {
                        v[i].first[0], remaining[i], v[i].first[2]
                    },
                    i
                });
                inserted[i] = true;
            }
        }

        if (!ready.empty()) {
            auto it = ready.begin();
            auto[vec, pid] = * it;
            ready.erase(it);

            mp[pid].push_back({
                time,
                time + 1
            });
            remaining[pid]--;
            time++;

            if (remaining[pid] > 0) {
                ready.insert({
                    {
                        time,
                        remaining[pid],
                        vec[2]
                    },
                    pid
                });
            } else {
                completed++;
            }
        } else {
            time++;
        }
    }

}
/*
1. We priority wise execute the tasks with categorization for different algorithms.
2. Here different priority queues don't interact with each other.

*/
/*
Multi level Queue:
1. Processes divided into queues based on the priority.
2. Once higher priority processes executed then only lower priority processes ececuted.
3. Different queues can have different algorithms.
4. They totally independent queues.
*/
void MLQ(vector < pair < vector < int > , int >> v, map < int, vector < pair < int, int >>> & mp) {
    map < int, vector < pair < int, int >>> mp1, mp2, mp3;
    vector < pair < vector < int > , int >> v1, v2, v3;
    int tot_bt1(0), tot_bt2(0);
    for (auto x: v) {
        int P = x.first[2];
        if (P >= 7) {
            v1.push_back(x);
            tot_bt1 += x.first[1];
        }
        else if (P < 7 && P >= 4) {
            v2.push_back(x);
            tot_bt2 += x.first[1];
        }
        else {
            v3.push_back(x);
        }
    }
    if (!v1.empty()) {
        Robin_Round(v1, mp1, 2);
        for (int i = 0; i < v2.size(); i++) v2[i].first[0] = tot_bt1;
    }
    if (!v2.empty()) {
        Robin_Round(v2, mp2, 4);
        //SRJF(v2,mp2);
        for (int i = 0; i < v3.size(); i++) v3[i].first[0] = (tot_bt1 + tot_bt2);
    }
    FCFS(v3, mp3);
    for (auto & m: {
            mp1,
            mp2,
            mp3
        }) {
        for (auto z: m) {
            for (auto x: z.second) {
                mp[z.first].push_back(x);
            }
        }
    }
}
/*
Multi Level Feedback Queue:
1. Queues are interacting with each other.
2. First all processes are fed into the highest priority queue.
3. Then they are executed for the specific time quantum, if they are not completed , we depromote them to the lower priority queue.
4. Once the higher priority queue is empty, lower priority queue starts executing the processes.
*/
void MLFQ(vector < pair < vector < int > , int >> v, map < int, vector < pair < int, int >>> & mp) {
    queue < pair < vector < int > , int >> ready1, ready2, ready3;
    int tq1(2), tq2(4);
    set < pair < vector < int > , int >> st(v.begin(), v.end());
    auto x = st.begin();
    ready1.push({
        * x
    });
    int curr = ( * st.begin()).first[0];
    st.erase(x);
    while (!ready1.empty()) {
        auto x = ready1.front();
        ready1.pop();
        int at = x.first[0];
        int bt = x.first[1];
        int id = x.second;
        curr = max(curr, at);
        mp[id].push_back({
            curr,
            curr + min(tq1, bt)
        });
        curr += min(tq1, bt);
        while (!st.empty() && ((curr >= ( * st.begin()).first[0]))) {
            ready1.push( * st.begin());
            st.erase(st.begin());
        }
        if (bt >= tq1) bt -= tq1;
        else bt = 0;
        if (bt > 0) ready2.push({
            {
                at,
                bt,
                x.first[2]
            },
            id
        });
    }
    while (!ready2.empty()) {
        auto x = ready2.front();
        ready2.pop();
        int at = x.first[0];
        int bt = x.first[1];
        int id = x.second;
        curr = max(curr, at);
        mp[id].push_back({
            curr,
            curr + min(tq2, bt)
        });
        curr += min(tq2, bt);
        if (bt >= tq2) bt -= tq2;
        else bt = 0;
        if (bt > 0) ready3.push({
            {
                at,
                bt,
                x.first[2]
            },
            id
        });
    }
    while (!ready3.empty()) {
        auto x = ready3.front();
        ready3.pop();
        int at = x.first[1];
        int bt = x.first[1];
        int id = x.second;
        curr = max(curr, at);
        mp[id].push_back({
            curr,
            curr + bt
        });
        curr += bt;
    }
}
// I have merged all intervals like (0,1),(1,2)=(0,2) for each id.
void merge_overlapping(map < int, vector < pair < int, int >>> & mp1) {
    map < int, vector < pair < int, int >>> mp = mp1;
    mp1.clear();

    for (auto & x: mp) {
        auto & intervals = x.second;
        sort(intervals.begin(), intervals.end());

        vector < pair < int, int >> merged;
        if (intervals.empty()) continue;

        int start = intervals[0].first;
        int end = intervals[0].second;

        for (int i = 1; i < intervals.size(); ++i) {
            if (intervals[i].first <= end) {
                // Merge overlapping or touching intervals
                end = max(end, intervals[i].second);
            } else {
                merged.push_back({
                    start,
                    end
                });
                start = intervals[i].first;
                end = intervals[i].second;
            }
        }
        merged.push_back({
            start,
            end
        });
        mp1[x.first] = merged;
    }
}
int main() {
    int n, choice;
    cout << "Enter number of processes: ";
    cin >> n;

    vector < pair < vector < int > , int >> v;

    cout << "Enter AT BT PRIORITY for each process:\n";
    for (int i = 0; i < n; i++) {
        int at, bt, pr;
        cout << "Process " << i << ": ";
        cin >> at >> bt >> pr;
        v.push_back({
            {
                at,
                bt,
                pr
            },
            i
        });
    }
    cout << "Choose Scheduling Algorithm:\n";
    cout << "1. FCFS\n2. SJF\n3. SRJF\n4. Priority_nonpremptive\n5. Priority_premptive\n6. Robin_Round\n7. MLQ\n8. MLFQ\n";
    cin >> choice;
    map < int, vector < pair < int, int >>> mp;
    if (choice == 1) {
        FCFS(v, mp);
    } else if (choice == 2) {
        SJF(v, mp);
    } else if (choice == 3) {
        SRJF(v, mp);
    } else if (choice == 4) {
        priority_non_premptive(v, mp);
    } else if (choice == 5) {
        priority_premptive(v, mp);
    } else if (choice == 6) {
        Robin_Round(v, mp, 2);
    } else if (choice == 7) {
        MLQ(v, mp);
    } else if (choice == 8) {
        MLFQ(v, mp);
    }
    else cout << "Invalid Input!" << endl;
    merge_overlapping(mp);
    for (auto x: mp) {
        cout << "Process " << x.first << ": ";
        for (auto[start, end]: x.second) {
            cout << "[" << start << ", " << end << "] ";
        }
        cout << endl;
    }
    map < int, vector < int >> ans; //WT,CT,TAT,RT
    cout << "Process: " << "|" << "CT" << "|" << "TAT" << "|" << "WT" << "|" << "RT" << endl;
    for (auto x: mp) {
        int rt = x.second[0].first - v[x.first].first[0];
        int ct = x.second[x.second.size() - 1].second;
        int tat = ct - v[x.first].first[0];
        int wt = tat - v[x.first].first[1];
        ans[x.first] = {
            ct,
            tat,
            wt,
            rt
        };
        cout << x.first << " " << ct << " " << tat << " " << wt << " " << rt << endl;

    }
    return 0;
}