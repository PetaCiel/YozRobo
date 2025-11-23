    var tabs = document.getElementById('tabcontroll').getElementsByTagName('a');
    var pages = document.getElementById('tabbody').getElementsByTagName('div');
    function changeTab(){
        var targetid = this.href.substring(this.href.indexOf('#')+1,this.href.length);
        for (var i=0; i<pages.length; i++){
            if(pages[i].id != targetid){
                pages[i].style.display = 'none';
            }
            else{
                pages[i].style.display = 'block';
            }
        }
        for (var i=0; i<tabs.length; i++){
            tabs[i].style.zIndex = "0";
        }
        this.style.zIndex = "10";
        return false;
    }
    for (var i=0; i<tabs.length; i++){
        tabs[i].onclick = changeTab;
    }

    tabs[0].onclick();

    var page2Button = document.getElementById('page2-button');
    if(page2Button){
        page2Button.addEventListener('click', function(){
            var input = document.getElementById('page2-input').value;
            var outputBox = document.getElementById('output-box');
            if (outputBox){
                outputBox.textContent = '入力された内容: ' + input;
            }
            alert("page2のボタンが押されたよん");
        }
    );
    }