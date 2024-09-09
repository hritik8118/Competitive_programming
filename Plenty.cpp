class Solution {
public:
    int moves[50][50][50][50];
    vector<pair<int, int>> directions = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
    map<pair<int, int>, bool> m;
    vector<vector<vector<int>>> dp; // 3D DP table
    bool already_computed = false;
    bool is_valid(int x, int y) {
        return (x >= 0 && x < 50 && y >= 0 && y < 50);
    }
    void Expand(int x, int y) {
        vector<vector<bool>> visited(50, vector<bool>(50, false));
        queue<pair<int, int>> q;
        moves[x][y][x][y] = 0;
        q.push({x, y});
        visited[x][y] = true;
        while (!q.empty()) {
            auto s = q.front();
            q.pop();
            for (const auto& dir : directions) {
                int nxt_x = dir.first + s.first;
                int nxt_y = dir.second + s.second;
                if (!is_valid(nxt_x, nxt_y) || visited[nxt_x][nxt_y]) continue;
                
                moves[x][y][nxt_x][nxt_y] = 1 + moves[x][y][s.first][s.second];
                q.push({nxt_x, nxt_y});
                visited[nxt_x][nxt_y] = true;
            }
        }
    }
    void CalculateAll() {
        if (already_computed) return;
        already_computed = true;
        for (int kx = 0; kx < 50; kx++) {
            for (int ky = 0; ky < 50; ky++) {
                Expand(kx, ky);
            }
        }
    }
    int solve(int last, int mask, int flag, vector<vector<int>>& positions) {
        int maxChoosableInteger = positions.size();
        if (dp[last][mask][flag] != INT_MIN) {
            return dp[last][mask][flag];
        }
        if (mask == (1 << maxChoosableInteger) - 1) {
            return 0;
        }
        int curr_x = positions[last][0];
        int curr_y = positions[last][1];
        int ans;
        if (flag == 0) { 
            ans = INT_MIN;
            for (int i = 0; i < maxChoosableInteger; i++) {
                if ((mask & (1 << i)) == 0) {
                    int x1 = positions[i][0];
                    int y1 = positions[i][1];
                    int new_mask = mask | (1 << i);
                    if (moves[curr_x][curr_y][x1][y1] > 0) { 
                        ans = max(ans, moves[curr_x][curr_y][x1][y1] + solve(i, new_mask, 1, positions));
                    }
                }
            }
        } else {
            ans = INT_MAX;
            for (int i = 0; i < maxChoosableInteger; i++) {
                if ((mask & (1 << i)) == 0) {
                    int x1 = positions[i][0];
                    int y1 = positions[i][1];
                    int new_mask = mask | (1 << i);
                    if (moves[curr_x][curr_y][x1][y1] > 0) {
                        ans = min(ans, moves[curr_x][curr_y][x1][y1] + solve(i, new_mask, 0, positions));
                    }
                }
            }
        }
        return dp[last][mask][flag] = ans;
    }
    int maxMoves(int kx, int ky, vector<vector<int>>& positions) {
        CalculateAll();
        int n = positions.size();
        dp.assign(n + 1, vector<vector<int>>(1 << (n + 1), vector<int>(2, INT_MIN)));
        positions.push_back({kx, ky});
        return solve(n, 1 << n, 0, positions);
    }
};
