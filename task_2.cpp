#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <ranges>
#include <cassert>

using namespace std;

bool IsPrefix(string_view prefix, string_view full) {
    return prefix == full.substr(0, prefix.size());
}

class Domain {
public:
    // разработайте класс домена
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain() = default;
    Domain(string_view data):
    data_(data) {
        data_ = "." + data_;
        reverse(data_.begin(), data_.end());
    }
    // разработайте operator==
    bool operator==(const Domain& other) const {
        return data_ == other.data_;
    }
    bool operator<(const Domain& other) const {
        return lexicographical_compare(data_.begin(), data_.end(), other.data_.begin(), other.data_.end());
    }
    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& other) const {
        return IsPrefix(data_, other.data_);
    }
    string GetData() const {
        return data_;
    }
private:
    string data_;
};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template <typename InputIt>
    DomainChecker(InputIt first, InputIt last):
    forbidden_domains_(first, last)
    {
        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        auto new_end = unique(forbidden_domains_.begin(), forbidden_domains_.end(),[](const Domain& lhs, const Domain& rhs){
            return lhs.IsSubdomain(rhs) || rhs.IsSubdomain(lhs);
        });
        forbidden_domains_.erase(new_end, forbidden_domains_.end());
    }
    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        if (it == forbidden_domains_.begin())
            return false;
        it = prev(it);
        if ((*it).IsSubdomain(domain) || (domain).IsSubdomain(*it))
            return true;
        return false;
    }
    vector<Domain> GetDomains() const {
        return forbidden_domains_;
    }
    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
private:
    vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
vector<Domain> ReadDomains(istream& input, size_t size) {
    vector<Domain> result;
    string domain;
    for (size_t i = 0; i < size; ++i) {
        if (getline(input, domain)) {
            result.emplace_back(Domain(domain));
        }
        continue;
    }
    return result;
}
template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

void TestDomainConstructor(){
    string data = "papiroska.rf";
    Domain domain(data);
    assert(domain.GetData() == "fr.aksoripap.");
}

void TestDomainEqualOp(){
    Domain d1("papiroska.rf");
    Domain d2("papiroska.rf");
    assert(d1 == d2);
}

void TestIsSubdomain() {
    {
        Domain domain("papiroska.rf");
        Domain subdomain("hqd.papiroska.rf");
        assert(domain.IsSubdomain(subdomain));
    }
    {
        Domain domain("papiroska.rf");
        Domain subdomain("hqdpapiroska.rf");
        assert(!domain.IsSubdomain(subdomain));
    }
}

void TestDomainReader() {
    istringstream data("gdz.ru\npapiroska.rf");
    auto v = ReadDomains(data, 2);
    assert(v[0] == Domain("gdz.ru"));
    assert(v[1] == Domain("papiroska.rf"));
}

void TestDomain(){
    TestDomainConstructor();
    TestDomainEqualOp();
    TestIsSubdomain();
    TestDomainReader();
}

void TestDomainCheckerConstructor() {
    {
        vector<Domain> v = {Domain("papiroska.rf"), Domain("gdz.ru")};
        DomainChecker checker(v.begin(), v.end());
        assert(checker.GetDomains().at(1) == Domain("gdz.ru"));
        assert(checker.GetDomains().at(0) == Domain("papiroska.rf"));
        assert(checker.GetDomains().size() == 2);
    }
    {
        vector<Domain> v = {Domain("papiroska.rf"), Domain("gdz.ru"), Domain("free.gdz.ru")};
        DomainChecker checker(v.begin(), v.end());
        assert(checker.GetDomains().at(1) == Domain("gdz.ru"));
        assert(checker.GetDomains().at(0) == Domain("papiroska.rf"));
        assert(checker.GetDomains().size() == 2);
    }
}

void TestDomainCheckerForbidden() {
    {
        vector<Domain> v = {Domain("papiroska.rf"), Domain("gdz.ru")};
        DomainChecker checker(v.begin(), v.end());
        assert(checker.IsForbidden(Domain("papiroska.rf")));
        assert(checker.IsForbidden(Domain("gdz.ru")));
        assert(checker.IsForbidden(Domain("free.gdz.ru")));
        assert(checker.IsForbidden(Domain("hqd.papiroska.rf")));
        assert(!checker.IsForbidden(Domain("freegdz.ru")));
        assert(!checker.IsForbidden(Domain("hqdpapiroska.rf")));
        assert(checker.IsForbidden(Domain("gdz.ru")));
        assert(checker.IsForbidden(Domain("papiroska.rf")));
    }
}

void TestDomainChecker(){
    TestDomainCheckerConstructor();
    TestDomainCheckerForbidden();
}

void TestExample(){
    istringstream data(R"(4
gdz.ru
maps.me
m.gdz.ru
com
7
gdz.ru
gdz.com
m.maps.me
alg.m.gdz.ru
maps.com
maps.ru
gdz.ua)");
    ostringstream out;
    const std::vector<Domain> forbidden_domains = ReadDomains(data, ReadNumberOnLine<size_t>(data));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    string actual_result;
    const std::vector<Domain> test_domains = ReadDomains(data, ReadNumberOnLine<size_t>(data));
    bool is_first = true;
    for (const Domain& domain : test_domains) {
        out << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
        actual_result = out.str();
    }
    string desired_result = R"(Bad
Bad
Bad
Bad
Bad
Good
Good
)";
    actual_result = out.str();
    assert(actual_result == desired_result);
}

void Test(){
    TestDomain();
    TestDomainChecker();
    TestExample();
}


int main() {
    Test();
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
