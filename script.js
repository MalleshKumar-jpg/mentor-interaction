function toggleEditMode(button) {
    const meetingCard = button.closest('.meeting-card');
    const noteContent = meetingCard.querySelector('p'); 
    const meetingDate = meetingCard.querySelector('.meeting-date');
    const editIcon = button.querySelector('img');      

    if (editIcon.getAttribute('alt') === 'Edit') {
        noteContent.contentEditable = 'true';    
        noteContent.focus();            
        noteContent.style.border = '2px solid #4CAF50'; 

        meetingDate.removeAttribute('readonly');
        meetingDate.style.border = '2px solid #4CAF50';  
                           
        editIcon.setAttribute('src', 'images/save_icon.png');
        editIcon.setAttribute('alt', 'Save');

    } else {
        const updatedText = noteContent.textContent.trim();
        const updatedDate = meetingDate.value;

        if (!updatedText) {
            alert('Note content cannot be empty.');
            return; 
        }
        if (!updatedDate) {
            alert('Please select a valid date.');
            return;
        }

        noteContent.contentEditable = 'false';        
        noteContent.style.border = 'none';      
        
        meetingDate.setAttribute('readonly', true);
        meetingDate.style.border = 'none';

        editIcon.setAttribute('src', 'images/pencil_edit.png');
        editIcon.setAttribute('alt', 'Edit');
    }
}

function addNewMeeting() {
    const meetingDate = document.querySelector('#meeting-date').value;
    const meetingSummary = document.querySelector('#meeting-summary').value.trim();

    if (meetingDate && meetingSummary) {
        const newMeetingCard = document.createElement('div');
        newMeetingCard.classList.add('meeting-card');
        newMeetingCard.innerHTML = `
            <input type="date" class="meeting-date" value="${meetingDate}" readonly>
            <p>${meetingSummary}</p>
            <button class="edit-note mentor-only-btn">
                <img src="images/pencil_edit.png" alt="Edit">
            </button>
            <button class="delete-meeting mentor-only-btn">🗑️</button> 
        `;

        document.querySelector('.meeting-history').appendChild(newMeetingCard);

        newMeetingCard.querySelector('.edit-note').addEventListener('click', function () {
            toggleEditMode(this);
        });

        newMeetingCard.querySelector('.delete-meeting').addEventListener('click', function () {
            deleteMeeting(this);
        });

        document.querySelector('#meeting-date').value = '';
        document.querySelector('#meeting-summary').value = '';
    } else {
        if (!meetingSummary) {
            alert('Note content cannot be empty.');
            return; 
        }
        if (!meetingDate) {
            alert('Please select a valid date.');
            return;
        }
    }
}

function deleteMeeting(button) {
    const meetingCard = button.closest('.meeting-card'); 
    if (confirm('Are you sure you want to delete this meeting?')) {
        meetingCard.remove();  
    }
}

window.onload = function() {
    const url_params= new URLSearchParams(window.location.search);
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    const role = url_params.get('role'); /* retrieving the type of user (mentor/mentee) from url*/
=======
    const role = url_params.get('role'); /* rtrieving the type of user (mentor/mentee) from url*/
>>>>>>> Stashed changes
=======
    const role = url_params.get('role'); /* rtrieving the type of user (mentor/mentee) from url*/
>>>>>>> Stashed changes
    
    if (document.body.id==='login-page'){
        document.getElementById('role').value = role;
        document.getElementById('login-form').addEventListener('submit', function (event){
        event.preventDefault();
        if (role){
            sessionStorage.setItem('role',role);
            event.preventDefault();
            if  (role==='mentor'){
                window.location.href = 'mentor_menteeslist.html';
            } else if (role==='mentee'){
                window.location.href = 'main.html';
            }
        }else{
            alert('No role detected. Please select your role')
        }
        });
    }

    if (document.body.id=="menteeslist-page"){
        const mentee_cards=document.querySelectorAll('.mentee-card');

        mentee_cards.forEach(card => {
            card.addEventListener('click',function(){
                const chosenMentee=card.getAttribute('data-mentee');
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
                console.log(`Chosen Mentee: ${chosenMentee}`);
>>>>>>> Stashed changes
=======
                console.log(`Chosen Mentee: ${chosenMentee}`);
>>>>>>> Stashed changes
                sessionStorage.setItem('chosen_mentee',chosenMentee)
            });
        });
    }

    /* hiding buttons and elements based on the type of user */
    if (document.body.id === 'main-page'){
        const storedRole=sessionStorage.getItem('role');

        if (storedRole == 'mentor'){
            document.querySelectorAll('.mentee-only-btn').forEach(btn => btn.style.display='none'); /* hiding the class of mentee-only-btn */
            const sidebar_mentee_name=document.getElementById('mentee-name');
            sidebar_mentee_name.textContent=sessionStorage.getItem('chosen_mentee');
<<<<<<< Updated upstream
<<<<<<< Updated upstream
            
=======

            /* Meeting History functionalising */
            document.querySelectorAll('.edit-note').forEach(button => {
                button.addEventListener('click', function() {
                    toggleEditMode(this);
                });
            });

            document.querySelector('#notes .save-btn').addEventListener('click', addNewMeeting);
            
=======

            /* Meeting History functionalising */
            document.querySelectorAll('.edit-note').forEach(button => {
                button.addEventListener('click', function() {
                    toggleEditMode(this);
                });
            });

            document.querySelector('#notes .save-btn').addEventListener('click', addNewMeeting);
            
>>>>>>> Stashed changes
            document.querySelectorAll('.delete-meeting').forEach(button => {
                button.addEventListener('click', function() {
                    deleteMeeting(this);
                });
            });
            
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
        }else if(storedRole == 'mentee'){
            document.querySelectorAll('.mentor-only-btn, .mentor-only-container').forEach(elem => elem.style.display='none'); /* hiding the class of mentor-only-btn , mentor-only container */
            
            const sidebar_mentee_name = document.getElementById('mentee-name'); /* hiding the sidebar mentee-name . we cant include this in above code , because mentee-name is an id*/
            sidebar_mentee_name.style.backgroundColor='darkgreen';
            sidebar_mentee_name.textContent='';

            const header_role=document.getElementById('user-role');
            header_role.textContent='MENTEE';
        }
        
    }
};
<<<<<<< Updated upstream
<<<<<<< Updated upstream
function toggleEditMode(button) {
    const meetingCard = button.closest('.meeting-card');
    const noteContent = meetingCard.querySelector('.note-content');

    if (button.textContent === '✏️') {
        noteContent.contentEditable = 'true'; 
        noteContent.focus(); 
        noteContent.style.border = '2px solid #4CAF50'; 
        button.textContent = '💾';
    } else {
        noteContent.contentEditable = 'false';  
        noteContent.style.border = 'none';
        button.textContent = '✏️'
    }
}
document.querySelectorAll('.edit-btn').forEach(button => {
    button.addEventListener('click', () => toggleEditMode(button));
});

function addNewMeeting() {
    const meetingDate = document.querySelector('#meeting-date').value;
    const meetingSummary = document.querySelector('#meeting-summary').value;

    if (meetingDate && meetingSummary) {
        const newMeetingCard = document.createElement('div');
        newMeetingCard.classList.add('meeting-card');
        newMeetingCard.innerHTML = `
            <h3>${meetingDate}</h3>
            <p class="note-content">${meetingSummary}</p>
            <button class="edit-btn">✏️</button>
        `;
        document.querySelector('#meeting-history').appendChild(newMeetingCard);
        newMeetingCard.querySelector('.edit-btn').addEventListener('click', () => toggleEditMode(newMeetingCard.querySelector('.edit-btn')));
        document.querySelector('#meeting-date').value = '';
        document.querySelector('#meeting-summary').value = '';
    } else {
        alert('Please fill in both the date and summary.');
    }
}

document.querySelector('#notes .save-btn').addEventListener('click', addNewMeeting);
=======

>>>>>>> Stashed changes
=======

>>>>>>> Stashed changes
