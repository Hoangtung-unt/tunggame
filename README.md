# Chase hit monster
-
# GIỚI THIỆU GAME 
Chase hit monster là một game làm từ sdl2 đơn giản với nội dung chính là nhân vật di chuyển trên map tạo sẵn bắn những con quái vật xuất hiện ngẫu nhiên
# 1. BẮT ĐẦU GAME
Chờ một chút màn hình sẽ hiện ra màn hình start yêu cầu người chới phải bấm enter để bắt đầu (lỗi vẫn chưa cải thiện nên chưa thể hiện ạ)
Sau đó game sẽ hiện ra một màn hình
![Screenshot 2025-04-26 053544](https://github.com/user-attachments/assets/b869b76a-e23a-4e60-905f-9bd3e14c4f65)
# 2. CÁCH GAME HOẠT ĐỘNG
Nhân vật sẽ được di chuyển bằng các phím mũi tên hướng lên trên là nhảy, hướng sang trái là di chuyển sáng trái,hướng sang phải là di chuyển sang phải
Nhân vật bắn đạn bằng cách bấm phím space
![image](https://github.com/user-attachments/assets/9bd8c9b0-fd07-4953-83fb-0489509425f9)
khi người chơi di chuyển để quái chạm vào nhân vật thì ngay lập tức sẽ out game và người chới sẽ bị xử thua
# 3. CÁCH CHIẾN THẮNG
khi người chơi đưa được nhân vật đến phần cuối của bản đồ thì người chơi đã chiến thắng
# Về đồ hoạ game 
- .png là để lấy ảnh trong suốt
- .ttf là font chữ để hiện bảng thông báo start
- .text đây là bảng tile với mỗi số tượng trưng cho 1 hình ảnh vdu dất cỏ dùng để tạo map di chuyển cho nhân vật
- các file ảnh được tìm trên ytb
# Về code game
- Chứa sdl2 sdl_ttf sdl_image dùng để tải ảnh tải chữ
- Map:dùng kĩ thuật tile map để tạo map vs mỗi 1 tile tượng trưng cho một hình ảnh
- bulet: đạn bắn ra từ nhân vật
- camera: dùng camera cuộn để có thể di chuyển map theo nhân vật
- enermy: quái vật mang hình dáng zombie được tạo ngẫu nhiên di chuyển theo hướng nhân vật
- player: nhân vật chính do người chơi điều khiển  


