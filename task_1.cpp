#include <iostream>
#include <vector>
#include <unordered_set>
#include <iomanip>
#include <cassert>
#include <sstream>
#include <algorithm>

using namespace std;

class Book {
public:
    void Read(int user, int page) {
        if (user_to_page_count_.size() <= user) {
            user_to_page_count_.resize(user + 1);
        }
        if (page_to_user_count_.size() <= page) 
            page_to_user_count_.resize(page + 1);
        for (size_t i = user_to_page_count_.at(user) + 1; i <= page; ++i) {
            ++page_to_user_count_[i];
        }
        user_to_page_count_[user] = page;
        users_.insert(user);
    }
    double Cheer(int user) {
        if (user_to_page_count_.size() <= user || user_to_page_count_.at(user) == 0)
            return 0;
        int page = user_to_page_count_.at(user);
        double user_count = UsersCount() - 1;
        int user_count_of_page = page_to_user_count_.at(page) - 1;
        if (user_count_of_page == 0)
            return 1;
        return (user_count - user_count_of_page) / user_count;
    }
private:
    double UsersCount() const {
        return users_.size();
    }
    vector<int> user_to_page_count_;
    vector<int> page_to_user_count_;
    unordered_set<int> users_;
};

int main() {
    Book book;
    int n;
    cin >> n;
    for (size_t i = 0; i < n; ++i) {
        string query_type;
        cin >> query_type;
        if (query_type == "READ"s) {
            int user, page;
            cin >> user >> page;
            book.Read(user, page);
        }
        if (query_type == "CHEER"s) {
            int user;
            cin >> user;
            cout << book.Cheer(user) << setprecision(6) << endl;
        }
    }
}
